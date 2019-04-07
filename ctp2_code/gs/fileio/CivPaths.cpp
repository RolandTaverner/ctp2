//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : File paths
// Id           : $Id$
//
//----------------------------------------------------------------------------
//
// Disclaimer
//
// THIS FILE IS NOT GENERATED OR SUPPORTED BY ACTIVISION.
//
// This material has been developed at apolyton.net by the Apolyton CtP2
// Source Code Project. Contact the authors at ctp2source@apolyton.net.
//
//----------------------------------------------------------------------------
//
// Compiler flags
//
// - None
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Added option to use multiple data directories.
// - Memory leak/crash fix
// - FindFile can ignore files in scenario paths. (9-Apr-2007 Martin Gühmann)
//
//----------------------------------------------------------------------------
#include "ctp/c3.h"

#include <fstream>

#include "ctp/ctp2_utils/c3files.h"
#include "gs/fileio/CivPaths.h"

namespace fs = std::filesystem;

#ifdef WIN32
#include <shlobj.h>
#endif

CivPaths *g_civPaths;

#include "gs/fileio/prjfile.h"
extern ProjectFile *g_ImageMapPF;


void CivPaths::InitCivPaths() {
  delete g_civPaths;
  g_civPaths = new CivPaths;
}


void CivPaths::CleanupCivPaths() {
  delete g_civPaths;
  g_civPaths = NULL;
}


CivPaths::CivPaths() {
  std::fill(m_desktopPath, m_desktopPath + _MAX_PATH, 0);

  std::ifstream fin("civpaths.txt");
  Assert(fin.good());

  std::getline(fin, m_hdPath);
  std::getline(fin, m_cdPath);
  std::getline(fin, m_defaultPath);
  std::getline(fin, m_localizedPath);
  std::getline(fin, m_dataPath);
  std::getline(fin, m_scenariosPath);
  std::getline(fin, m_savePath);
  std::getline(fin, m_saveGamePath);
  std::getline(fin, m_saveQueuePath);
  std::getline(fin, m_saveMPPath);
  std::getline(fin, m_saveSCENPath);
  std::getline(fin, m_saveMapPath);
  std::getline(fin, m_saveClipsPath);

  for (size_t dir = 0; dir < C3DIR_MAX; ++dir) {
    std::getline(fin, m_assetPaths[dir]);
  }

  fin.close();

  fs::path fullPath = fs::absolute(fs::path(m_hdPath) / m_savePath);

  CreateSaveFolders(fullPath);
}


CivPaths::~CivPaths() {}


void CivPaths::CreateSaveFolders(const fs::path &path) {
  fs::create_directories(path);

  fs::create_directory(path / m_saveGamePath);

  fs::create_directory(path / m_saveQueuePath);

  fs::create_directory(path / m_saveMPPath);

  fs::create_directory(path / m_saveSCENPath);

  fs::create_directory(path / m_saveMapPath);

  fs::create_directory(path / m_saveClipsPath);
}


void CivPaths::InitCDPath(void) {
  MBCHAR tempPath[_MAX_PATH];
  sprintf(tempPath, "%c:%s%s", c3files_GetCtpCdId(), FILE_SEP, m_cdPath.c_str());
  m_cdPath = tempPath;
}


std::string CivPaths::MakeSavePath(const std::string &s1, const std::string &s2, const std::string &s3) {
  fs::path fullPath = fs::absolute(fs::path(s1) / s2 / s3);

  if (fs::exists(fullPath)) {
    return fullPath.string() + FILE_SEP;
  }
  return std::string();
}


const MBCHAR *CivPaths::GetSavePath(C3SAVEDIR dir, MBCHAR *path) {
  std::string fullPath;

  switch (dir) {
    case C3SAVEDIR_GAME:
      fullPath = MakeSavePath(m_hdPath, m_savePath, m_saveGamePath);
      break;
    case C3SAVEDIR_QUEUES:
      fullPath = MakeSavePath(m_hdPath, m_savePath, m_saveQueuePath);
      break;
    case C3SAVEDIR_MP:
      fullPath = MakeSavePath(m_hdPath, m_savePath, m_saveMPPath);
      break;
    case C3SAVEDIR_SCEN:
      fullPath = MakeSavePath(m_hdPath, m_savePath, m_saveSCENPath);
      break;
    case C3SAVEDIR_MAP:
      fullPath = MakeSavePath(m_hdPath, m_savePath, m_saveMapPath);
      break;
    case C3SAVEDIR_CLIPS:
      fullPath = MakeSavePath(m_hdPath, m_savePath, m_saveClipsPath);
      break;
    default:
      Assert(FALSE);
  }

  if (!fullPath.empty()) {
    strcpy(path, fullPath.c_str());
    return path;
  }
  return NULL;
}

std::string CivPaths::MakeAssetPath(const std::string &s1,
  const std::string &s2,
  const std::string &s3,
  const std::string &s4,
  const std::string &s5) {
  fs::path fullPath = fs::absolute(fs::path(s1) / s2 / s3 / s4 / s5);

  if (fs::exists(fullPath)) {
    return fullPath.string();
  }
  return std::string();
}


std::string CivPaths::FindFile(C3DIR dir, const std::string &filename,
  bool silent, bool check_prjfile, bool checkScenario) {
  Assert(dir < C3DIR_MAX);
  Assert(!filename.empty());
  if (dir == C3DIR_DIRECT) {
    return filename;
  }

  if (checkScenario) {
    if (!m_curScenarioPath.empty()) {
      fs::path fullPath = fs::path(m_curScenarioPath) / m_localizedPath / m_assetPaths[dir] / filename;
      if (c3files_PathIsValid(fullPath)) {
        return fullPath.string();
      }
      fullPath = fs::path(m_curScenarioPath) / m_defaultPath / m_assetPaths[dir] / filename;
      if (c3files_PathIsValid(fullPath)) {
        return fullPath.string();
      }
    }

    if (!m_curScenarioPackPath.empty()) {
      fs::path fullPath = fs::path(m_curScenarioPackPath) / m_localizedPath / m_assetPaths[dir] / filename;
      if (c3files_PathIsValid(fullPath)) {
        return fullPath.string();
      }
      fullPath = fs::path(m_curScenarioPackPath) / m_defaultPath / m_assetPaths[dir] / filename;
      if (c3files_PathIsValid(fullPath)) {
        return fullPath.string();
      }
    }
  }

  // The extra data paths take priority over the regular one.
  for (auto &p : m_extraDataPaths) {
    std::string fullPath = MakeAssetPath(m_hdPath,
      p,
      m_localizedPath,
      m_assetPaths[dir],
      filename);

    if (!fullPath.empty()) {
      return fullPath;
    }
    fullPath = MakeAssetPath(m_hdPath,
      p,
      m_localizedPath,
      m_assetPaths[dir],
      filename);
    if (!fullPath.empty()) {
      return fullPath;
    }
  }

  std::string fullPath;

  // When not found in the new data, try the original directories
  fullPath = MakeAssetPath(m_hdPath, m_dataPath, m_localizedPath, m_assetPaths[dir], filename);
  if (!fullPath.empty()) {
    return fullPath;
  }
  fullPath = MakeAssetPath(m_hdPath, m_dataPath, m_defaultPath, m_assetPaths[dir], filename);
  if (!fullPath.empty()) {
    return fullPath;
  }

  // The CD will only have the original content
  fullPath = MakeAssetPath(m_cdPath, m_dataPath, m_localizedPath, m_assetPaths[dir], filename);
  if (!fullPath.empty()) {
    return fullPath;
  }
  fullPath = MakeAssetPath(m_cdPath, m_dataPath, m_defaultPath, m_assetPaths[dir], filename);
  if (!fullPath.empty()) {
    return fullPath;
  }

  if (check_prjfile &&
    ((dir == C3DIR_PATTERNS) ||
    (dir == C3DIR_PICTURES))) {

    const std::size_t len = filename.length();

    if (len > 3) {
      fullPath = filename;
      fullPath[len - 3] = 'r';
      fullPath[len - 2] = 'i';
      fullPath[len - 1] = 'm';

      if (g_ImageMapPF && g_ImageMapPF->exists(fullPath.c_str())) {
        return filename;
      }
    }
  }

  if (!silent)
    c3errors_ErrorDialog("Paths", "'%s' not found in asset tree.", filename);

  return std::string();
}


//----------------------------------------------------------------------------
//
// Name       : CivPaths::FindPath
//
// Description: Get the next possible lookup path
//
// Parameters : dir		type of path to lookup (not really used)
//              num     "index" of path to lookup
//
// Returns    : bool    there may be more paths to lookup
//              path    filled with a found path (set to "" when not found)
//
// Remark(s)  : -
//
//----------------------------------------------------------------------------

bool CivPaths::FindPath(C3DIR dir, int num, std::string &path) {
  Assert(dir < C3DIR_MAX);
  path.clear();

  if (dir == C3DIR_DIRECT) {
    return false;
  }

  fs::path tempPath;

  switch (num) {
    case 0:
      if (!m_curScenarioPath.empty()) {
        tempPath = fs::path(m_curScenarioPath) / m_localizedPath / m_assetPaths[dir];
      }
      break;
    case 1:
      if (!m_curScenarioPath.empty()) {
        tempPath = fs::path(m_curScenarioPath) / m_defaultPath / m_assetPaths[dir];
      }
      break;
    case 2:
      if (!m_curScenarioPath.empty()) {
        tempPath = fs::path(m_curScenarioPackPath) / m_localizedPath / m_assetPaths[dir];
      }
      break;
    case 3:
      if (!m_curScenarioPath.empty()) {
        tempPath = fs::path(m_curScenarioPackPath) / m_defaultPath / m_assetPaths[dir];
      }
      break;
    default:
    {
      size_t const i = (num - 4) / 2;

      if (i < m_extraDataPaths.size()) {
        if (num & 1)    // even: language dependent, odd: default
        {
          tempPath = fs::path(m_hdPath) / m_extraDataPaths[i] / m_defaultPath / m_assetPaths[dir];
        } else {
          tempPath = fs::path(m_hdPath) / m_extraDataPaths[i] / m_localizedPath / m_assetPaths[dir];
        }
      } else {
        switch (num - 2 * m_extraDataPaths.size()) {
          case 4:
            tempPath = fs::path(m_hdPath) / m_dataPath / m_localizedPath / m_assetPaths[dir];
            break;
          case 5:
            tempPath = fs::path(m_hdPath) / m_dataPath / m_defaultPath / m_assetPaths[dir];
            break;
          case 6:
            if (!m_cdPath.empty()) {
              tempPath = fs::path(m_cdPath) / m_dataPath / m_localizedPath / m_assetPaths[dir];
            }
            break;
          case 7:
            if (!m_cdPath.empty()) {
              tempPath = fs::path(m_cdPath) / m_dataPath / m_defaultPath / m_assetPaths[dir];
            }
            break;
          default:
            return false;
        } // switch

        tempPath = fs::absolute(tempPath);
        if (!fs::exists(tempPath)) {
          tempPath.clear();
        }
      }
    } // scope default
  } // switch

  path = tempPath.string();
  return true;
}


std::string CivPaths::GetSpecificPath(C3DIR dir, BOOL local) {
  Assert(dir < C3DIR_MAX);
  if (dir >= C3DIR_MAX) 
    return std::string();

  fs::path tempPath;

  if (local) {
    tempPath = fs::path(m_hdPath) / m_dataPath / m_localizedPath / m_assetPaths[dir];
  } else {
    tempPath = fs::path(m_hdPath) / m_dataPath / m_defaultPath / m_assetPaths[dir];
  }
  const fs::path fullPath = fs::absolute(tempPath);

  return fullPath.string();
}

std::string CivPaths::GetScenarioRootPath() {
  fs::path fullPath = fs::absolute(fs::path(m_scenariosPath));
  Assert(fs::exists(fullPath));

  return fullPath.string();
}

void CivPaths::SetCurScenarioPath(const MBCHAR *path) {
  m_curScenarioPath = path;
}

const std::string &CivPaths::GetCurScenarioPath(void) {
  return m_curScenarioPath;
}

void CivPaths::ClearCurScenarioPath(void) {
  m_curScenarioPath.clear();
}

void CivPaths::SetCurScenarioPackPath(const MBCHAR *path) {
  m_curScenarioPackPath = path;
}

const std::string &CivPaths::GetCurScenarioPackPath(void) {
  return m_curScenarioPackPath;
}

void CivPaths::ClearCurScenarioPackPath(void) {
  m_curScenarioPackPath.clear();
}





const MBCHAR *CivPaths::GetDesktopPath(void) {
#ifdef WIN32
  MBCHAR		    tempStr[_MAX_PATH] = { 0 };
  ITEMIDLIST *    idList;
  HRESULT		    hr = SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &idList);

  Assert(hr == S_OK);
  if ((hr != S_OK)
    || !SHGetPathFromIDList(idList, tempStr)
    ) {
    MBCHAR * s = _fullpath(tempStr, FILE_SEP, _MAX_PATH);
    Assert(s);
    if (!s) return NULL;
  }

  strcpy(m_desktopPath, tempStr);
  return m_desktopPath;
#else
  return NULL;
#endif
}

//----------------------------------------------------------------------------
//
// Name       : CivPaths::InsertExtraDataPath
//
// Description: Insert a data include directory to the lookup path
//
// Parameters : path	: ctp2_data-style directory tree
//
// Globals    : -
//
// Returns    : -
//
// Remark(s)  : For lookup, the most recent path has the highest priority,
//              and any inserted paths have priority above the original
//              "..\..\ctp2_data" path.
//
//----------------------------------------------------------------------------
void CivPaths::InsertExtraDataPath(MBCHAR const * path) {
  m_extraDataPaths.insert(m_extraDataPaths.begin(), std::string(path));
}
