//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Music settings screen
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
// - Cleanup improved.
// - Don't reset user changes when returning from the track selection window.
// - Initialized local variables. (Sep 9th 2005 Martin G�hmann)
//
//----------------------------------------------------------------------------

#include "ctp/c3.h"
#include "ui/interface/musicscreen.h"

#include "ui/aui_common/aui_stringtable.h"
#include "ui/aui_common/aui_uniqueid.h"
#include "ui/aui_ctp2/c3_button.h"
#include "ui/aui_ctp2/c3_checkbox.h"
#include "ui/aui_ctp2/c3_dropdown.h"
#include "ui/aui_ctp2/c3_listitem.h"
#include "ui/aui_ctp2/c3_popupwindow.h"
#include "ui/aui_ctp2/c3_static.h"
#include "ui/aui_ctp2/c3_switch.h"
#include "ui/aui_ctp2/c3slider.h"
#include "ui/aui_ctp2/c3ui.h"
#include "ui/aui_ctp2/c3window.h"
#include "ui/aui_ctp2/ctp2_button.h"
#include "gs/utility/Globals.h"            // allocated::clear
#include "ui/aui_ctp2/keypress.h"
#include "ui/interface/musictrackscreen.h"
#include "gs/database/profileDB.h"          // g_theProfileDB
#include "sound/soundmanager.h"       // g_soundManager
#include "ui/interface/spnewgamewindow.h"

extern C3UI *   g_c3ui;
extern BOOL     g_musicTrackChosen;

static c3_PopupWindow	*s_musicScreen	= NULL;
static ctp2_Button		*s_selectTrack	= NULL;

static c3_Switch		*s_autoRepeat	= NULL,
						*s_randomOrder	= NULL,
						*s_musicOn		= NULL;

static aui_StringTable	*s_musicString	= NULL;

static BOOL				s_useAutoRepeat = FALSE;
static BOOL				s_useRandomOrder = FALSE;
static BOOL				s_useMusicOn = FALSE;




sint32	musicscreen_displayMyWindow()
{
	sint32 retval = (s_musicScreen) ? 0 : musicscreen_Initialize();

	s_autoRepeat->SetState(s_useAutoRepeat);
	s_randomOrder->SetState(s_useRandomOrder);
	s_musicOn->SetState(s_useMusicOn);

	AUI_ERRCODE auiErr = g_c3ui->AddWindow(s_musicScreen);
	Assert( auiErr == AUI_ERRCODE_OK );
	keypress_RegisterHandler(s_musicScreen);

	return retval;
}
sint32 musicscreen_removeMyWindow(uint32 action)
{
	if ( action != (uint32)AUI_BUTTON_ACTION_EXECUTE ) return 0;

	AUI_ERRCODE auiErr = g_c3ui->RemoveWindow(s_musicScreen->Id());
	Assert( auiErr == AUI_ERRCODE_OK );
	keypress_RemoveHandler(s_musicScreen);

	return 1;
}


AUI_ERRCODE musicscreen_Initialize( void )
{
	if ( s_musicScreen ) return AUI_ERRCODE_OK;

	AUI_ERRCODE errcode = AUI_ERRCODE_OK;
	MBCHAR		windowBlock[ k_AUI_LDL_MAXBLOCK + 1 ];

	strcpy(windowBlock, "MusicScreen");
	{
		s_musicScreen = new c3_PopupWindow( &errcode, aui_UniqueId(), windowBlock, 16, AUI_WINDOW_TYPE_FLOATING, false );
		Assert( AUI_NEWOK(s_musicScreen, errcode) );
		if ( !AUI_NEWOK(s_musicScreen, errcode) )
		{
			return errcode;
		}

		s_musicScreen->Resize(s_musicScreen->Width(),s_musicScreen->Height());
		s_musicScreen->GrabRegion()->Resize(s_musicScreen->Width(),s_musicScreen->Height());
		s_musicScreen->SetStronglyModal(TRUE);
	}


	s_musicScreen->AddTitle( "MusicScreen.Name" );
	s_musicScreen->AddClose( musicscreen_acceptPress );

	s_selectTrack = spNew_ctp2_Button( &errcode, windowBlock, "SelectTrackButton", musicscreen_selectTrackPress );

	s_musicString = spNewStringTable( &errcode, "MS_Strings" );

	s_autoRepeat	= spNew_c3_Switch(&errcode,windowBlock,"AutoRepeatSwitch",musicscreen_checkPress );
	s_autoRepeat->SetText( s_musicString->GetString(MS_STRING_AUTO_OFF) );
	s_randomOrder	= spNew_c3_Switch(&errcode,windowBlock,"RandomOrderSwitch",musicscreen_checkPress );
	s_randomOrder->SetText( s_musicString->GetString(MS_STRING_RANDOM_OFF) );
	s_musicOn		= spNew_c3_Switch(&errcode,windowBlock,"MusicOnSwitch",musicscreen_checkPress );

	s_useAutoRepeat		= g_soundManager->IsAutoRepeat();
	s_useRandomOrder	= (MUSICSTYLE_RANDOM == g_soundManager->GetMusicStyle());
	s_useMusicOn		= g_theProfileDB->IsUseRedbookAudio();

	errcode = aui_Ldl::SetupHeirarchyFromRoot( windowBlock );
	Assert( AUI_SUCCESS(errcode) );

	return AUI_ERRCODE_OK;
}


void musicscreen_Cleanup()
{
	musictrackscreen_Cleanup();

	if (s_musicScreen)
	{
		g_c3ui->RemoveWindow(s_musicScreen->Id());
		keypress_RemoveHandler(s_musicScreen);
	}

	allocated::clear(s_musicString);
	allocated::clear(s_selectTrack);
	allocated::clear(s_autoRepeat);
	allocated::clear(s_randomOrder);
	allocated::clear(s_musicOn);
	allocated::clear(s_musicScreen);
}

void musicscreen_checkPress(aui_Control *control, uint32 action, uint32 data, void *cookie )
{
	c3_Switch *musicSwitch = (c3_Switch *)control;

	switch ( action ) {
	case AUI_SWITCH_ACTION_ON:
		if ( musicSwitch == s_autoRepeat ) {
			s_autoRepeat->SetText( s_musicString->GetString(MS_STRING_AUTO_ON) );
			s_useAutoRepeat = TRUE;
		}
		else if ( musicSwitch == s_randomOrder ) {
			s_randomOrder->SetText( s_musicString->GetString(MS_STRING_RANDOM_ON) );
			s_useRandomOrder = TRUE;
		}
		else if ( musicSwitch == s_musicOn ) {
			s_useMusicOn = TRUE;
		}
		break;

	case AUI_SWITCH_ACTION_OFF:
		if ( musicSwitch == s_autoRepeat ) {
			s_autoRepeat->SetText( s_musicString->GetString(MS_STRING_AUTO_OFF) );
			s_useAutoRepeat = FALSE;
		}
		else if ( musicSwitch == s_randomOrder ) {
			s_randomOrder->SetText( s_musicString->GetString(MS_STRING_RANDOM_OFF) );
			s_useRandomOrder = FALSE;
		}
		else if ( musicSwitch == s_musicOn ) {
			s_useMusicOn = FALSE;
		}
		break;
	}
}
void musicscreen_selectTrackPress(aui_Control *control, uint32 action, uint32 data, void *cookie )
{
	if ( musicscreen_removeMyWindow(action) ) {
		musictrackscreen_displayMyWindow();
	}
}
void musicscreen_acceptPress(aui_Control *control, uint32 action, uint32 data, void *cookie )
{
	if ( action != (uint32)AUI_BUTTON_ACTION_EXECUTE ) return;

	if (g_soundManager) {
		g_soundManager->SetAutoRepeat(s_useAutoRepeat);

		if (g_musicTrackChosen) {
			g_soundManager->SetMusicStyle(MUSICSTYLE_USER);
			g_soundManager->SetLastTrack(0);
		} else {
			if (s_useRandomOrder)
				g_soundManager->SetMusicStyle(MUSICSTYLE_RANDOM);
			else
				g_soundManager->SetMusicStyle(MUSICSTYLE_PLAYLIST);
		}

		if ( s_useMusicOn ) {
			g_soundManager->EnableMusic();
			g_soundManager->InitRedbook();
		} else {
			g_soundManager->DisableMusic();
			g_soundManager->CleanupRedbook();
		}

		g_theProfileDB->SetUseRedbookAudio(s_useMusicOn);

		g_soundManager->PickNextTrack();
		g_soundManager->StartMusic();
	}

	musicscreen_removeMyWindow(action);

	g_musicTrackChosen = FALSE;
}
