#include "ctp/c3.h"
#include "gs/slic/slicif.h"
#include "gs/fileio/CivPaths.h"
#include "ctp/ctp2_utils/c3errors.h"
#include "gs/database/StrDB.h"

extern CivPaths *g_civPaths;
extern StringDB *g_theStringDB;

int slicif_find_file(char *filename, char *fullpath) {
  const std::string fp = g_civPaths->FindFile(C3DIR_GAMEDATA, filename);
  if (fp.empty()) {
    return 0;
  }
  strcpy(fullpath, fp.c_str());
	return 1;
}

void slicif_report_error(char *s)
{
	c3errors_ErrorDialog("SLIC", s);
}

int slicif_is_valid_string(char *s)
{
	return g_theStringDB->GetNameStr(s) != NULL;
}
