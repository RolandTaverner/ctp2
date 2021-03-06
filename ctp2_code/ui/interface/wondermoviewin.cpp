//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Wonder movie window
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
// - Initialized local variables. (Sep 9th 2005 Martin G�hmann)
//
//----------------------------------------------------------------------------

#include "ctp/c3.h"

#include "ui/aui_ctp2/c3ui.h"
#include "ui/aui_common/aui_uniqueid.h"
#include "ui/aui_common/aui_button.h"

#include "ui/aui_ctp2/c3window.h"

#include "ui/interface/wondermoviewindow.h"
#include "ui/interface/wondermoviewin.h"

#include "gs/newdb/WonderRecord.h"
#include "gs/newdb/WonderMovieRecord.h"
#include "gs/database/StrDB.h"

#include "sound/soundmanager.h"

#include "gfx/spritesys/director.h"
#include "gs/newdb/IconRecord.h"
#include "ui/interface/greatlibrary.h"
#include "gs/slic/SlicObject.h"

extern C3UI				*g_c3ui;
extern SoundManager		*g_soundManager;

WonderMovieWindow		*g_wonderMovieWindow = NULL;


void wondermoviewin_Initialize(SequenceWeakPtr seq)
{
	AUI_ERRCODE		errcode = AUI_ERRCODE_OK;

	if (g_wonderMovieWindow == NULL) {
		g_wonderMovieWindow = new WonderMovieWindow(&errcode, aui_UniqueId(), "WonderMovieWindow", 16);
		Assert(errcode == AUI_ERRCODE_OK);

		g_wonderMovieWindow->SetSequence(seq);
	}
}


void wondermoviewin_DisplayWonderMovie(sint32 id)
{
	if (g_soundManager) {
		g_soundManager->TerminateAllSounds();
		g_soundManager->TerminateMusic();
	}

	Assert(g_wonderMovieWindow != NULL);
	if (g_wonderMovieWindow == NULL) return;

	WonderRecord *rec = g_theWonderDB->Access(id);
	Assert(rec);






	const MBCHAR *filename = rec->GetDefaultIcon()->GetMovie();
	if(stricmp(filename, "NULL") == 0)
		filename = NULL;

	g_wonderMovieWindow->SetMovie((char *)filename);

	MBCHAR *name = (MBCHAR *)g_theStringDB->GetNameStr(rec->GetName());
	g_wonderMovieWindow->SetWonderName(name);

	SlicObject so;
	so.AddWonder(id);
	const MBCHAR *glText = glutil_LoadText(rec->GetDefaultIcon()->GetGameplay(), so);

	g_wonderMovieWindow->SetText(glText);

	AUI_ERRCODE		errcode;

	errcode = g_c3ui->AddWindow(g_wonderMovieWindow);
	Assert(errcode == AUI_ERRCODE_OK);

}


void wondermoviewin_Cleanup()
{
  SequenceWeakPtr seq;

  if (g_wonderMovieWindow) {
    seq = g_wonderMovieWindow->GetSequence();

		g_c3ui->RemoveWindow(g_wonderMovieWindow->Id());

		delete g_wonderMovieWindow;
		g_wonderMovieWindow = NULL;
	}

	g_director->ActionFinished(seq);
}


void wondermoviewin_MovieButtonCallback(aui_Control *control, uint32 action, uint32 data, void * cookie)
{
	if ( action != (uint32)AUI_BUTTON_ACTION_EXECUTE ) return;

	g_c3ui->AddAction(new CloseMovieAction);
}


void CloseMovieAction::Execute(aui_Control *control, uint32 action, uint32 data)
{


	wondermoviewin_Cleanup();

	if (g_soundManager) {
		g_soundManager->StartMusic();
	}
}
