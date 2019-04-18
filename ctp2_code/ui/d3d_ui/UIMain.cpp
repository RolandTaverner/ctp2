#include "ctp/c3.h"

#include <fstream>

#include "ui/d3d_ui/UIMain.h"

namespace ui::d3d {

UIMain::UIMain() {}

UIMain::~UIMain() {}

void UIMain::Initialize(HINSTANCE hinst, int cmdshow, unsigned windowWidth, unsigned windowHeight, const std::string &ldlFile) {
  UI::InitializeUI(hinst, cmdshow, windowWidth, windowHeight, ldlFile);

  FileInfo fi = GetFileManager()->FindFile(ldlFile, PATH_LAYOUT);
  FileAccessorUPtr fa = std::move(fi.GetAccessor());
  std::size_t s = fa->GetSize();


}

void UIMain::InitPaths(const std::string &pathsFileName, const ExtraDataPaths &extraDataPaths) {
  std::ifstream pathsFile(pathsFileName);
  if (!pathsFile) {
    throw std::runtime_error("Can't open " + pathsFileName);
  }

  std::string hdPath;
  if (!std::getline(pathsFile, hdPath)) {
    throw std::runtime_error("Can't read hdPath from " + pathsFileName);
  }

  std::string cdPath;
  if (!std::getline(pathsFile, cdPath)) {
    throw std::runtime_error("Can't read cdPath from " + pathsFileName);
  }

  std::string defaultPath;
  if (!std::getline(pathsFile, defaultPath)) {
    throw std::runtime_error("Can't read defaultPath from " + pathsFileName);
  }

  std::string localizedPath;
  if (!std::getline(pathsFile, localizedPath)) {
    throw std::runtime_error("Can't read localizedPath from " + pathsFileName);
  }

  std::string dataPath;
  if (!std::getline(pathsFile, dataPath)) {
    throw std::runtime_error("Can't read dataPath from " + pathsFileName);
  }

  std::string scenariosPath;
  if (!std::getline(pathsFile, scenariosPath)) {
    throw std::runtime_error("Can't read scenariosPath from " + pathsFileName);
  }

  std::string savePath;
  if (!std::getline(pathsFile, savePath)) {
    throw std::runtime_error("Can't read savePath from " + pathsFileName);
  }

  std::string saveGamePath;
  if (!std::getline(pathsFile, saveGamePath)) {
    throw std::runtime_error("Can't read saveGamePath from " + pathsFileName);
  }

  std::string saveQueuePath;
  if (!std::getline(pathsFile, saveQueuePath)) {
    throw std::runtime_error("Can't read saveQueuePath from " + pathsFileName);
  }

  std::string saveMPPath;
  if (!std::getline(pathsFile, saveMPPath)) {
    throw std::runtime_error("Can't read saveMPPath from " + pathsFileName);
  }

  std::string saveSCENPath;
  if (!std::getline(pathsFile, saveSCENPath)) {
    throw std::runtime_error("Can't read saveSCENPath from " + pathsFileName);
  }

  std::string saveMapPath;
  if (!std::getline(pathsFile, saveMapPath)) {
    throw std::runtime_error("Can't read saveMapPath from " + pathsFileName);
  }

  std::string saveClipsPath;
  if (!std::getline(pathsFile, saveClipsPath)) {
    throw std::runtime_error("Can't read saveClipsPath from " + pathsFileName);
  }

  std::map<PathContentTag, std::string> assetPaths;
  for (int i = 0; i < PATH_MAX; ++i) {
    std::string assetPath;
    if (!std::getline(pathsFile, assetPath)) {
      throw std::runtime_error("Can't read assetPath from " + pathsFileName);
    }
    assetPaths[(PathContentTag)i] = assetPath;
  }

  // TODO: determine full CD path
  GetFileManager()->Initialize(hdPath, "", defaultPath, localizedPath, dataPath, scenariosPath, assetPaths, extraDataPaths);

  GetFileManager()->AddZFSFileSearchPath("pat565.zfs", PATH_PATTERNS, true);
  GetFileManager()->AddZFSFileSearchPath("pic565.zfs", PATH_PICTURES, true);
  // TODO: add sound etc.
}

void UIMain::SetCurScenarioPath(const std::string &path) {
  GetFileManager()->SetCurScenarioPath(path);
}

const std::string &UIMain::GetCurScenarioPath() {
  return GetFileManager()->GetCurScenarioPath();
}

void UIMain::SetCurScenarioPackPath(const std::string &path) {
  GetFileManager()->SetCurScenarioPackPath(path);
}

const std::string &UIMain::GetCurScenarioPackPath() {
  return GetFileManager()->GetCurScenarioPackPath();
}

} // namespace ui::d3d