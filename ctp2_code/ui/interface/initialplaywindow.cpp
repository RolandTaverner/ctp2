
#include "c3.h"


#include "aui.h"
#include "aui_ldl.h"
#include "aui_surface.h"
#include "aui_uniqueid.h"
#include "aui_imagebase.h"
#include "aui_textbase.h"
#include "aui_textfield.h"
#include "aui_stringtable.h"
#include "c3ui.h"
#include "c3window.h"
#include "c3_Button.h"
#include "c3_static.h"
#include "c3_listitem.h"
#include "c3_dropdown.h"
#include "StrDB.h"
#include "CivilisationDB.h"
#include "civilisationpool.h"

#include "ctp2_Button.h"

#include "spnewgamewindow.h" 
#include "initialplaywindow.h"

extern StringDB						*g_theStringDB;
extern CivilisationDatabase			*g_theCivilisationDB;








#include "ctp2_static.h"

InitPlayWindow::InitPlayWindow(AUI_ERRCODE *retval, uint32 id,
		MBCHAR *ldlBlock, sint32 bpp, AUI_WINDOW_TYPE type, bool bevel)
		: C3Window(retval,id,ldlBlock,bpp,type,bevel),
		m_sp(NULL), m_mp(NULL), m_load(NULL), m_continue(NULL), m_instant(NULL),
		m_mapeditor(NULL), m_quit(NULL), m_background(NULL), m_email(NULL),
		m_hotseat(NULL)
{
	ctp2_Static *testBox=new ctp2_Static(retval, aui_UniqueId(),"InitPlayWindow.TestTextBox");


	
 	m_spriteTest = spNew_ctp2_Button(retval,
		   							ldlBlock, 
		   							"SpriteTestButton",
		   							"Sprite Test",
		   							spritetest_spPress,
		   							"CTP2_BUTTON_TEXT_RIGHT_LARGE");
#ifndef _DEBUG
	m_spriteTest->Hide();
#endif

	
 	m_sp = spNew_ctp2_Button(retval,
		   					 ldlBlock, 
		   					 "SPButton",
		   					 "Single Player",
		   					 initialplayscreen_spPress,
		   					 "CTP2_BUTTON_TEXT_RIGHT_LARGE");

	
	m_email	= spNew_ctp2_Button(retval,
								ldlBlock, 
								"EmailButton",
								"Email",
								initialplayscreen_emailPress,
								"CTP2_BUTTON_TEXT_LEFT_LARGE");
	
	
	
	m_hotseat = spNew_ctp2_Button(retval,
								  ldlBlock, 
								  "HotseatButton",
								  "Hotseat",
								  initialplayscreen_hotseatPress,
								  "CTP2_BUTTON_TEXT_LEFT_LARGE");
   
	
	
	m_mp = spNew_ctp2_Button(retval,
							 ldlBlock, 
							 "MPButton",
							 "Multiplayer",
							 initialplayscreen_mpPress,
							 "CTP2_BUTTON_TEXT_LEFT_LARGE");

	m_quit = spNew_ctp2_Button(retval,
							   ldlBlock,
							   "QuitButton",
							   "Quit Out",
							   initialplayscreen_quitPress,
							   "CTP2_BUTTON_TEXT_LEFT_LARGE");

	m_credits = spNew_ctp2_Button(retval,
								  ldlBlock,
								  "CreditsButton",
								  "Credits",
								  initialplayscreen_creditsPress,
								  "CTP2_BUTTON_TEXT_RIGHT_LARGE");






	
	m_background = spNew_c3_Static(retval,ldlBlock,"Background");
}


InitPlayWindow::~InitPlayWindow()
{
#define mycleanup(mypointer) if(mypointer) { delete mypointer; mypointer = NULL; };
	mycleanup(m_spriteTest);
	mycleanup(m_sp);
	mycleanup(m_mp);




	mycleanup(m_quit);
	mycleanup(m_credits);
	mycleanup(m_hotseat);
	mycleanup(m_email);
	
	mycleanup(m_background);
#undef mycleanup
}

