#include "ctp/c3.h"

#include "gs/fileio/CivPaths.h"
#include "ctp/fingerprint/verify.h"
#include "ctp/fingerprint/ctp_finger.h"

extern CivPaths		*g_civPaths;

BOOL ctpfinger_Check(void)
{
	BOOL	success = FALSE;
	std::string	fingerprintPath;
  std::string	userListPath;

  fingerprintPath = g_civPaths->FindFile(k_FINGERPRINT_ASSET_DIR, k_FINGERPRINT_ASSET);
	if (fingerprintPath.empty())
		return FALSE;

  userListPath = g_civPaths->FindFile(k_USER_LIST_ASSET_DIR, k_USER_LIST_ASSET);
	if (userListPath.empty())
		return FALSE;

	if (GetInfoFromFingerprint(fingerprintPath.c_str()) && IsValidUser(userListPath.c_str()))
		success = TRUE;

	return success;
}
