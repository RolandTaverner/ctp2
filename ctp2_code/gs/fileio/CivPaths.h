//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ header
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
// _MSC_VER
// - Compiler version (for the Microsoft C++ compiler only)
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Added option to use multiple data directories.
// - FindFile can ignore files in scenario paths. (9-Apr-2007 Martin Gühmann)
//
//----------------------------------------------------------------------------

#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif

#ifndef __CIVPATHS_H__
#define __CIVPATHS_H__ 1

#include <filesystem>
#include <string>
#include <vector> // list did not work: crashes on begin() for empty list

#include "ctp/ctp2_utils/c3files.h"


class CivPaths {
public:

  CivPaths();

  virtual ~CivPaths();

  static void InitCivPaths();
  static void CleanupCivPaths();

  void CreateSaveFolders(const std::filesystem::path &path);

  void InitCDPath(void);

  const MBCHAR *GetSavePath(C3SAVEDIR dir, MBCHAR *path);

  std::string FindFile(C3DIR dir, const std::string &filename, bool silent = false, bool check_prjfile = true, bool checkScenario = true);

  std::string GetSpecificPath(C3DIR dir, BOOL local);

  std::string GetScenarioRootPath();

  void SetCurScenarioPath(const MBCHAR *path);

  const std::string &GetCurScenarioPath(void);

  void ClearCurScenarioPath(void);

  void SetCurScenarioPackPath(const MBCHAR *path);

  const std::string &GetCurScenarioPackPath(void);

  void ClearCurScenarioPackPath(void);

  bool FindPath(C3DIR dir, int num, std::string &path);

  const std::string &GetSavePathString(void) const { return m_savePath; }

  const MBCHAR *    GetDesktopPath(void);
  
  std::vector<std::string> const &GetExtraDataPaths(void) const;
  
  void InsertExtraDataPath(MBCHAR const * path);

protected:
  std::string MakeAssetPath(const std::string &s1,
    const std::string &s2,
    const std::string &s3,
    const std::string &s4,
    const std::string &s5);

  std::string MakeSavePath(const std::string &s1, const std::string &s2, const std::string &s3);

private:
  std::string m_hdPath;
  std::string m_cdPath;

  std::string m_defaultPath;
  std::string m_localizedPath;

  std::string m_dataPath;                       // original data path (...\ctp2_data)
  std::vector<std::string> m_extraDataPaths;      // searched before m_dataPath
  std::string m_scenariosPath;

  std::string m_savePath;
  std::string m_saveGamePath;
  std::string m_saveQueuePath;
  std::string m_saveMPPath;
  std::string m_saveSCENPath;
  std::string m_saveMapPath;
  std::string m_saveClipsPath;

  std::string m_assetPaths[C3DIR_MAX];

  std::string m_curScenarioPath;
  std::string m_curScenarioPackPath;

  MBCHAR m_desktopPath[_MAX_PATH];
};

extern CivPaths *g_civPaths;

#endif
