#pragma once

#include <map>
#include <memory>
#include <vector>
#include <set>
#include <string>
#include <tuple>
#include <vector>

namespace ui::d3d {

enum PathContentTag {
  PATH_DIRECT = -1,
  PATH_GAMEDATA,
  PATH_GL,
  PATH_AIDATA,
  PATH_UIDATA,
  PATH_LAYOUT,
  PATH_FONTS,
  PATH_GRAPHICS,
  PATH_SPRITES,
  PATH_TILES,
  PATH_PATTERNS,
  PATH_PICTURES,
  PATH_ICONS,
  PATH_CURSORS,
  PATH_SOUNDS,
  PATH_VIDEOS,
  PATH_MAX
};

enum PathLocationTag {
  PATHLOC_CURSCENARIO,
  PATHLOC_CURSCENARIOPACK,
  PATHLOC_EXTRA,
  PATHLOC_HD,
  PATHLOC_CD,
  PATHLOC_ZFS
};

class ResourceFileManager;
typedef std::shared_ptr<ResourceFileManager> ResourceFileManagerPtr;
typedef std::weak_ptr<ResourceFileManager> ResourceFileManagerWPtr;

class FileAccessor {
protected:
  FileAccessor();

public:
  virtual ~FileAccessor();

  virtual std::size_t GetSize() const = 0;
  virtual const void *GetContent() = 0;
};

typedef std::unique_ptr<FileAccessor> FileAccessorUPtr;

class FileInfo {
protected:
  FileInfo(ResourceFileManagerPtr manager, const std::string &fileName, int pathEntryID);

public:
  FileInfo(const FileInfo &rhs);
  FileInfo(FileInfo &&rhs);
  FileInfo &operator=(const FileInfo &rhs);
  FileInfo &operator=(FileInfo &&rhs);

  virtual ~FileInfo();

  const std::string &GetFileName() const;
  std::string GetFullPath() const;
  bool IsInZFS() const;
  std::string GetZFSPath() const;
  std::size_t GetFileSize() const;
  operator bool() const;

  FileAccessorUPtr GetAccessor();

private:
  int m_pathEntryID;
  ResourceFileManagerWPtr m_manager;
  std::string m_fileName;

  friend class ResourceFileManager;
};

class PathEntry;
typedef std::shared_ptr<PathEntry> PathEntryPtr;

typedef std::set<PathContentTag> PathContentTags;

typedef std::map<PathContentTag, std::string> AssetPathsMap;
typedef std::vector<std::string> ExtraDataPaths;

enum LocalizedOption {
  LOCOPT_BOTH = 0,
  LOCOPT_LOCALIZED_ONLY,
  LOCOPT_DEFAULT_ONLY
};

class ResourceFileManager : public std::enable_shared_from_this<ResourceFileManager> {
public:
  ResourceFileManager();
  virtual ~ResourceFileManager();
  
  void Initialize(
    const std::string &hdPath,
    const std::string &cdPath,
    const std::string &defaultPath,
    const std::string &localizedPath,
    const std::string &dataPath,
    const std::string &scenariosPath,
    const AssetPathsMap &assetPaths,
    const ExtraDataPaths &extraDataPaths);

  void AddFileSearchPath(const std::string &path, PathLocationTag pathLoc, bool localized, const PathContentTags &tags);
  void AddZFSFileSearchPath(const std::string &zfsFileName, PathContentTag tag, bool mmapAccess);

  FileInfo FindFile(const std::string &fileName, PathContentTag pathTag = PATH_DIRECT, LocalizedOption locOpt = LOCOPT_BOTH, bool searchZFS = true, bool checkScenario = true);
  std::string GetFullPath(int entryID, const std::string &fileName);
  bool IsInZFS(int entryID);
  std::string GetZFSPath(int entryID) const;
  std::size_t GetFileSize(int entryID, const std::string &fileName) const;
  FileAccessorUPtr GetFileAccessor(int entryID, const std::string &fileName);

  void SetCurScenarioPath(const std::string &path);
  const std::string &GetCurScenarioPath() const;
  void SetCurScenarioPackPath(const std::string &path);
  const std::string &GetCurScenarioPackPath() const;

private:
  typedef std::vector<std::tuple<std::string, PathLocationTag, bool> > ContentPaths;

  ContentPaths ResourceFileManager::FindContentPaths(PathContentTag tag);
  void RemoveLocation(PathLocationTag loc);

private:
  int m_lastEntryID;
  typedef std::map<int, PathEntryPtr> PathEntriesMap;
  PathEntriesMap m_pathEntriesMap;
  typedef std::vector<PathEntryPtr> PathEntries;
  PathEntries m_pathEntries;

  std::string m_curScenarioPath, m_curScenarioPackPath;
  std::string m_hdPath, m_cdPath;
  std::string m_localizedPath, m_defaultPath;
  std::string m_dataPath;
  std::string m_scenariosPath;
  AssetPathsMap m_assetsPaths;
  ExtraDataPaths m_extraDataPaths;
};

} // namespace ui::d3d