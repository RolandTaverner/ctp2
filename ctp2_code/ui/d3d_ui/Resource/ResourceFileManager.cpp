#include "ctp/c3.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/scoped_array.hpp>

#include "ui/d3d_ui/Resource/ResourceFileManager.h"
#include "ui/d3d_ui/Resource/ZFSFile.h"

namespace fs = std::filesystem;

namespace ui::d3d {

//------------------------------------------------------------------------------
// FileAccessor
//------------------------------------------------------------------------------

FileAccessor::FileAccessor() {}

FileAccessor::~FileAccessor() {}


class OSFileAccessor : public FileAccessor {
public:
  explicit OSFileAccessor(const fs::path filePath) :
    m_filePath(filePath) {
    const std::size_t fileSize = (std::size_t)fs::file_size(m_filePath);
    std::ifstream inFile(m_filePath, std::ifstream::in | std::ifstream::binary);
    if (!inFile) {
      throw std::runtime_error("Can't open " + m_filePath.string());
    }
    m_buf.reset(new char[fileSize]);
    if (!inFile.read(m_buf.get(), fileSize)) {
      m_buf.reset();
      throw std::runtime_error("Can't read file " + m_filePath.string());
    }
    m_size = fileSize;
  }

  virtual ~OSFileAccessor() {}

  std::size_t GetSize() const override {
    return m_size;
  }

  const void *GetContent() override {
    if (!GetSize() || !m_buf) {
      throw std::runtime_error("Empty file buffer");
    }
    return m_buf.get();
  }

private:
  fs::path m_filePath;
  std::size_t m_size = 0u;
  boost::scoped_array<char> m_buf;
};

class ZFSFileAccessor : public FileAccessor {
public:
  explicit ZFSFileAccessor(ZFSDataUPtr data) :
    m_data(std::move(data)) {}

  virtual ~ZFSFileAccessor() {}

  std::size_t GetSize() const override {
    if (!m_data) {
      return 0;
    }

    return m_data->GetSize();
  }

  const void *GetContent() override {
    if (!GetSize()) {
      throw std::runtime_error("Empty file buffer");
    }
    return m_data->GetContent();
  }

private:
  ZFSDataUPtr m_data;
};

//------------------------------------------------------------------------------
// FileInfo
//------------------------------------------------------------------------------

FileInfo::FileInfo(ResourceFileManagerPtr manager, const std::string &fileName, int pathEntryID) :
  m_manager(manager), m_fileName(fileName), m_pathEntryID(pathEntryID) {}

FileInfo::FileInfo(const FileInfo &rhs) {
  *this = rhs;
}

FileInfo::FileInfo(FileInfo &&rhs) {
  *this = std::move(rhs);
}

FileInfo &FileInfo::operator=(const FileInfo &rhs) {
  if (this != &rhs) {
    m_fileName = rhs.m_fileName;
    m_pathEntryID = rhs.m_pathEntryID;
    m_manager = rhs.m_manager;
  }
  return *this;
}

FileInfo &FileInfo::operator=(FileInfo && rhs) {
  if (this != &rhs) {
    m_fileName = std::move(rhs.m_fileName);
    m_pathEntryID = rhs.m_pathEntryID;
    m_manager = std::move(rhs.m_manager);
  }
  return *this;
}

FileInfo::~FileInfo() {}

const std::string &FileInfo::GetFileName() const {
  return m_fileName;
}

std::string FileInfo::GetFullPath() const {
  if (m_manager.expired()) {
    throw std::runtime_error("Error: " + m_fileName + " not found");
  }
  ResourceFileManagerPtr manager = m_manager.lock();
  return manager->GetFullPath(m_pathEntryID, GetFileName());
}

bool FileInfo::IsInZFS() const {
  if (m_manager.expired()) {
    throw std::runtime_error("Error: " + m_fileName + " not found");
  }
  ResourceFileManagerPtr manager = m_manager.lock();
  return manager->IsInZFS(m_pathEntryID);
}

std::string FileInfo::GetZFSPath() const {
  if (m_manager.expired()) {
    throw std::runtime_error("Error: " + m_fileName + " not found");
  }
  ResourceFileManagerPtr manager = m_manager.lock();
  return manager->GetZFSPath(m_pathEntryID);
}

std::size_t FileInfo::GetFileSize() const {
  if (m_manager.expired()) {
    throw std::runtime_error("Error: " + m_fileName + " not found");
  }
  ResourceFileManagerPtr manager = m_manager.lock();
  return manager->GetFileSize(m_pathEntryID, m_fileName);
}

FileInfo::operator bool() const {
  return !m_manager.expired();
}

FileAccessorUPtr FileInfo::GetAccessor() {
  if (m_manager.expired()) {
    throw std::runtime_error("Error: " + m_fileName + " not found");
  }
  ResourceFileManagerPtr manager = m_manager.lock();

  return manager->GetFileAccessor(m_pathEntryID, m_fileName);
}

//------------------------------------------------------------------------------
// PathEntry
//------------------------------------------------------------------------------

class PathEntry {
public:
  PathEntry(int id, const PathContentTags &tags, PathLocationTag pathLoc, bool localized) :
    m_tags(tags), m_pathLocation(pathLoc), m_id(id), m_localized(localized) {}
  virtual ~PathEntry() {}

  bool HasTag(PathContentTag tag) const {
    return m_tags.find(tag) != m_tags.end();
  }

  PathLocationTag GetLocationTag() const {
    return m_pathLocation;
  }

  int GetID() const {
    return m_id;
  }

  bool operator<(const PathEntry &rhs) const {
    if (GetLocationTag() != rhs.GetLocationTag()) {
      return GetLocationTag() < rhs.GetLocationTag();
    }
    return GetID() < rhs.GetID();
  }

  virtual bool HasFile(const std::string &fileName) = 0;

  virtual std::string GetFullPath(const std::string &fileName) = 0;

  virtual bool IsInZFS() = 0;

  virtual std::string GetZFSPath() const = 0;

  virtual std::size_t GetFileSize(const std::string &fileName) const = 0;

  virtual FileAccessorUPtr GetAccessor(const std::string &fileName) = 0;

  bool IsLocalized() const {
    return m_localized;
  }

private:
  PathContentTags m_tags;
  PathLocationTag m_pathLocation;
  int m_id;
  bool m_localized;
};

//------------------------------------------------------------------------------
// OSPathEntry
//------------------------------------------------------------------------------

class OSPathEntry : public PathEntry {
public:
  OSPathEntry(int id, const PathContentTags &tags, PathLocationTag pathLoc, bool localized, const fs::path &fullPath) :
    PathEntry(id, tags, pathLoc, localized), m_directory(fullPath) {}
  virtual ~OSPathEntry() {}

  bool HasFile(const std::string &fileName) override {
    const fs::path filePath = m_directory / fileName;
    return fs::exists(filePath) && fs::is_regular_file(filePath);
  }

  std::string GetFullPath(const std::string &fileName) override {
    return (m_directory / fileName).string();
  }

  bool IsInZFS() override {
    return false;
  }

  std::string GetZFSPath() const override {
    throw std::runtime_error("OSPathEntry has no ZFSPath");
  }

  std::size_t GetFileSize(const std::string &fileName) const {
    const fs::path filePath(m_directory / fileName);
    return (std::size_t)fs::file_size(filePath);
  }

  FileAccessorUPtr GetAccessor(const std::string &fileName) override {
    return std::move(std::make_unique<OSFileAccessor>(m_directory / fileName));
  }

private:
  fs::path m_directory;
};

//------------------------------------------------------------------------------
// ZFSPathEntry
//------------------------------------------------------------------------------

class ZFSPathEntry : public PathEntry {
public:
  ZFSPathEntry(int id, const PathContentTags &tags, bool localized, const fs::path &zfsPath, bool useMmap) :
    PathEntry(id, tags, PATHLOC_ZFS, localized), m_zfsFilePath(zfsPath), m_useMmap(useMmap) {
    // TODO: implement std::ifstream-based ZFS file
    m_ZFSFile = std::make_unique<ZFSFileMapped>(zfsPath.string());
  }

  virtual ~ZFSPathEntry() {}

  bool HasFile(const std::string &fileName) override {
    const std::string name = boost::algorithm::to_lower_copy(fileName);
    return m_ZFSFile->Exists(name);
  }

  std::string GetFullPath(const std::string &fileName) override {
    throw std::runtime_error("ZFSPathEntry has no FullPath");
  }

  bool IsInZFS() override {
    return true;
  }

  std::string GetZFSPath() const override {
    return m_zfsFilePath.string();
  }

  std::size_t GetFileSize(const std::string &fileName) const {
    const std::string name = boost::algorithm::to_lower_copy(fileName);
    if (!m_ZFSFile->Exists(name)) {
      return 0;
    }
    return m_ZFSFile->GetData(name)->GetSize();
  }

  FileAccessorUPtr GetAccessor(const std::string &fileName) override {
    const std::string name = boost::algorithm::to_lower_copy(fileName);
    if (!m_ZFSFile->Exists(name)) {
      return FileAccessorUPtr();
    }

    return std::make_unique<ZFSFileAccessor>(std::move(m_ZFSFile->GetData(name)));
  }

private:
  fs::path m_zfsFilePath;
  bool m_useMmap;
  ZFSFileBaseUPtr m_ZFSFile;
};

//------------------------------------------------------------------------------
// ResourceFileManager
//------------------------------------------------------------------------------

ResourceFileManager::ResourceFileManager() : m_lastEntryID(0) {}

ResourceFileManager::~ResourceFileManager() {}

void ResourceFileManager::Initialize(const std::string &hdPath,
  const std::string &cdPath,
  const std::string &defaultPath,
  const std::string &localizedPath,
  const std::string &dataPath,
  const std::string &scenariosPath,
  const AssetPathsMap &assetPaths,
  const ExtraDataPaths &extraDataPaths) {
  m_hdPath = hdPath;
  m_cdPath = cdPath;
  m_defaultPath = defaultPath;
  m_localizedPath = localizedPath;
  m_dataPath = dataPath;
  m_scenariosPath = scenariosPath;
  m_assetsPaths = assetPaths;
  m_extraDataPaths = extraDataPaths;

  for (int i = PATH_GAMEDATA; i < PATH_MAX; ++i) {
    const PathContentTag contentTag = (PathContentTag)i;
    auto paths = FindContentPaths(contentTag);
    for (const auto &path : paths) {
      PathContentTags tags{ contentTag };
      AddFileSearchPath(std::get<0>(path), std::get<1>(path), std::get<2>(path), tags);
    }
  }
}

bool PathEntryLess(const PathEntryPtr &lhs, const PathEntryPtr &rhs) {
  return *lhs < *rhs;
}

void ResourceFileManager::AddFileSearchPath(const std::string &path, PathLocationTag pathLoc, bool localized, const PathContentTags &tags) {
  const fs::path fullPath = fs::absolute(fs::path(path));
  if (!fs::exists(fullPath) || !fs::is_directory(fullPath)) {
    return;
  }

  PathContentTags useTags(tags);
  if (useTags.count(PATH_PATTERNS) || useTags.count(PATH_ICONS)) {
    useTags.insert(PATH_PICTURES);
  }

  PathEntryPtr newEntry(std::make_unique<OSPathEntry>(++m_lastEntryID, useTags, pathLoc, localized, fullPath));

  if (!m_pathEntries.empty()) {
    m_pathEntries.insert(
      std::upper_bound(m_pathEntries.begin(), m_pathEntries.end(), newEntry, PathEntryLess),
      newEntry);
  } else {
    m_pathEntries.push_back(newEntry);
  }

  m_pathEntriesMap.insert(std::make_pair(newEntry->GetID(), newEntry));
}

void ResourceFileManager::AddZFSFileSearchPath(const std::string &zfsFileName, PathContentTag tag, bool mmapAccess) {
  auto paths = FindContentPaths(tag);
  for (const auto &path : paths) {
    const fs::path zfsFullPath = fs::absolute(fs::path(std::get<0>(path)) / zfsFileName);
    if (!(fs::exists(zfsFullPath) && fs::is_regular_file(zfsFullPath))) {
      continue;
    }

    PathContentTags tags{ tag };
    if (tags.count(PATH_PATTERNS) || tags.count(PATH_ICONS)) {
      tags.insert(PATH_PICTURES);
    }
    const PathLocationTag locTag = PATHLOC_ZFS;
    const bool localized = std::get<2>(path);

    PathEntryPtr newEntry(std::make_unique<ZFSPathEntry>(++m_lastEntryID, tags, localized, zfsFullPath, mmapAccess));

    if (!m_pathEntries.empty()) {
      m_pathEntries.insert(
        std::upper_bound(m_pathEntries.begin(), m_pathEntries.end(), newEntry, PathEntryLess),
        newEntry);
    } else {
      m_pathEntries.push_back(newEntry);
    }

    m_pathEntriesMap.insert(std::make_pair(newEntry->GetID(), newEntry));
  }
}

ResourceFileManager::ContentPaths ResourceFileManager::FindContentPaths(PathContentTag tag) {
  const std::string assetPath = m_assetsPaths[tag];

  ContentPaths foundPaths;

  if (!m_curScenarioPath.empty()) {
    const fs::path localizedPath = fs::path(m_curScenarioPath) / m_localizedPath / assetPath;
    foundPaths.push_back(std::make_tuple(localizedPath.string(), PATHLOC_CURSCENARIO, true));

    const fs::path defaultPath = fs::path(m_curScenarioPath) / m_defaultPath / assetPath;
    foundPaths.push_back(std::make_tuple(defaultPath.string(), PATHLOC_CURSCENARIO, false));
  }

  if (!m_curScenarioPackPath.empty()) {
    const fs::path localizedPath = fs::path(m_curScenarioPackPath) / m_localizedPath / assetPath;
    foundPaths.push_back(std::make_tuple(localizedPath.string(), PATHLOC_CURSCENARIOPACK, true));

    const fs::path defaultPath = fs::path(m_curScenarioPackPath) / m_defaultPath / assetPath;
    foundPaths.push_back(std::make_tuple(defaultPath.string(), PATHLOC_CURSCENARIOPACK, false));
  }

  for (const std::string &extraPath : m_extraDataPaths) {
    const fs::path localizedPath = fs::path(m_hdPath) / extraPath / m_localizedPath / assetPath;
    foundPaths.push_back(std::make_tuple(localizedPath.string(), PATHLOC_EXTRA, true));

    const fs::path defaultPath = fs::path(m_hdPath) / extraPath / m_defaultPath / assetPath;
    foundPaths.push_back(std::make_tuple(defaultPath.string(), PATHLOC_EXTRA, false));
  }

  if (!m_dataPath.empty()) {
    const fs::path localizedPath = fs::path(m_hdPath) / m_dataPath / m_localizedPath / assetPath;
    foundPaths.push_back(std::make_tuple(localizedPath.string(), PATHLOC_HD, true));

    const fs::path defaultPath = fs::path(m_hdPath) / m_dataPath / m_defaultPath / assetPath;
    foundPaths.push_back(std::make_tuple(defaultPath.string(), PATHLOC_HD, false));

    if (!m_cdPath.empty()) {
      const fs::path localizedPath = fs::path(m_cdPath) / m_dataPath / m_localizedPath / assetPath;
      foundPaths.push_back(std::make_tuple(localizedPath.string(), PATHLOC_CD, true));

      const fs::path defaultPath = fs::path(m_cdPath) / m_dataPath / m_defaultPath / assetPath;
      foundPaths.push_back(std::make_tuple(defaultPath.string(), PATHLOC_CD, false));
    }
  }

  return foundPaths;
}

FileInfo ResourceFileManager::FindFile(const std::string &fileName, PathContentTag pathTag, LocalizedOption locOpt, bool searchZFS, bool checkScenario) {
  // Try regular files
  for (PathEntryPtr entry : m_pathEntries) {
    if (entry->GetLocationTag() == PATHLOC_ZFS) {
      continue;
    }
    if (locOpt == LOCOPT_LOCALIZED_ONLY && !entry->IsLocalized()) {
      continue;
    }
    if (locOpt == LOCOPT_DEFAULT_ONLY && entry->IsLocalized()) {
      continue;
    }

    if (entry->HasTag(pathTag) && entry->HasFile(fileName)) {
      return FileInfo(shared_from_this(), fileName, entry->GetID());
    }
  }
  
  // Try ZFS
  if (searchZFS) {
    const std::string rimFileName = fs::path(fileName).replace_extension("rim").string();
    for (PathEntryPtr entry : m_pathEntries) {
      if (entry->GetLocationTag() != PATHLOC_ZFS) {
        continue;
      }
      if (locOpt == LOCOPT_LOCALIZED_ONLY && !entry->IsLocalized()) {
        continue;
      }
      if (locOpt == LOCOPT_DEFAULT_ONLY && entry->IsLocalized()) {
        continue;
      }

      if (entry->HasTag(pathTag) && entry->HasFile(rimFileName)) {
        return FileInfo(shared_from_this(), fileName, entry->GetID());
      }
    }
  }

  return FileInfo(ResourceFileManagerPtr(), fileName, -1);
}

std::string ResourceFileManager::GetFullPath(int entryID, const std::string &fileName) {
  PathEntriesMap::const_iterator it = m_pathEntriesMap.find(entryID);
  if (it == m_pathEntriesMap.end()) {
    throw std::range_error("Unknown entry id=" + std::to_string(entryID) + " requested");
  }

  return it->second->GetFullPath(fileName);
}

bool ResourceFileManager::IsInZFS(int entryID) {
  PathEntriesMap::const_iterator it = m_pathEntriesMap.find(entryID);
  if (it == m_pathEntriesMap.end()) {
    throw std::range_error("Unknown entry id=" + std::to_string(entryID) + " requested");
  }

  return it->second->IsInZFS();
}

std::string ResourceFileManager::GetZFSPath(int entryID) const {
  PathEntriesMap::const_iterator it = m_pathEntriesMap.find(entryID);
  if (it == m_pathEntriesMap.end()) {
    throw std::range_error("Unknown entry id=" + std::to_string(entryID) + " requested");
  }

  return it->second->GetZFSPath();
}

std::size_t ResourceFileManager::GetFileSize(int entryID, const std::string &fileName) const {
  PathEntriesMap::const_iterator it = m_pathEntriesMap.find(entryID);
  if (it == m_pathEntriesMap.end()) {
    throw std::range_error("Unknown entry id=" + std::to_string(entryID) + " requested");
  }

  return it->second->GetFileSize(fileName);
}

FileAccessorUPtr ResourceFileManager::GetFileAccessor(int entryID, const std::string &fileName) {
  PathEntriesMap::const_iterator it = m_pathEntriesMap.find(entryID);
  if (it == m_pathEntriesMap.end()) {
    throw std::range_error("Unknown entry id=" + std::to_string(entryID) + " requested");
  }

  return it->second->GetAccessor(fileName);
}

void ResourceFileManager::RemoveLocation(PathLocationTag loc) {
  for (PathEntries::iterator it = m_pathEntries.begin(); it != m_pathEntries.end();) {
    if ((*it)->GetLocationTag() == loc) {
      m_pathEntriesMap.erase((*it)->GetID());
    }
  }

  m_pathEntries.erase(
    std::remove_if(
      m_pathEntries.begin(),
      m_pathEntries.end(),
      [loc](PathEntryPtr &e) { return e->GetLocationTag() == loc; }),
    m_pathEntries.end());
}

void ResourceFileManager::SetCurScenarioPath(const std::string &path) {
  RemoveLocation(PATHLOC_CURSCENARIO);

  m_curScenarioPath = path;

  if (!m_curScenarioPath.empty()) {
    for (auto assetPath : m_assetsPaths) {
      const fs::path localizedPath = fs::path(m_curScenarioPath) / m_localizedPath / assetPath.second;
      const fs::path defaultPath = fs::path(m_curScenarioPath) / m_defaultPath / assetPath.second;
      PathContentTags tags{ assetPath.first };

      AddFileSearchPath(localizedPath.string(), PATHLOC_CURSCENARIO, true, tags);
      AddFileSearchPath(defaultPath.string(), PATHLOC_CURSCENARIO, false, tags);
    } // for (auto assetPath : m_assetsPaths) 
  }
}

const std::string &ResourceFileManager::GetCurScenarioPath() const {
  return m_curScenarioPath;
}

void ResourceFileManager::SetCurScenarioPackPath(const std::string & path) {
  RemoveLocation(PATHLOC_CURSCENARIOPACK);

  m_curScenarioPackPath = path;

  if (!m_curScenarioPackPath.empty()) {
    for (auto assetPath : m_assetsPaths) {
      const fs::path localizedPath = fs::path(m_curScenarioPackPath) / m_localizedPath / assetPath.second;
      const fs::path defaultPath = fs::path(m_curScenarioPackPath) / m_defaultPath / assetPath.second;
      PathContentTags tags{ assetPath.first };

      AddFileSearchPath(localizedPath.string(), PATHLOC_CURSCENARIOPACK, true, tags);
      AddFileSearchPath(defaultPath.string(), PATHLOC_CURSCENARIOPACK, false, tags);
    } // for (auto assetPath : m_assetsPaths) 
  }
}

const std::string &ResourceFileManager::GetCurScenarioPackPath() const {
  return m_curScenarioPackPath;
}


} // namespace ui::d3d