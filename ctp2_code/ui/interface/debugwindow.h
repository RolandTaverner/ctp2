#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef __DEBUGWINDOW_H__
#define __DEBUGWINDOW_H__

#include "ui/aui_ctp2/c3window.h"

class TextBox;

class aui_TabGroup;

class DebugWindow : public C3Window
{
    uint32 m_debug_mask;
    BOOL m_allow_next;
public:
	DebugWindow(
		AUI_ERRCODE *retval,
		uint32 id,
		MBCHAR *ldlBlock,
		sint32 bpp,
		AUI_WINDOW_TYPE type = AUI_WINDOW_TYPE_BACKGROUND );
	DebugWindow(
		AUI_ERRCODE *retval,
		uint32 id,
		sint32 x,
		sint32 y,
		sint32 width,
		sint32 height,
		sint32 bpp,
		MBCHAR *pattern,
		AUI_WINDOW_TYPE type = AUI_WINDOW_TYPE_BACKGROUND );

	virtual ~DebugWindow() {}

	virtual AUI_ERRCODE InitCommon(void);

	virtual AUI_ERRCODE DrawThis(
		aui_Surface *surface = NULL,
		sint32 x = 0,
		sint32 y = 0 );






	void SetTextBox(TextBox *box) { m_textBox = box; }
	TextBox *GetTextBox(void) const { return m_textBox; }
	void AddText(MBCHAR *text);

    void SetDebugMask(uint32 m);
    void AddText(const uint32 m, MBCHAR *text);

    void AddMask(uint32 m);
    void AddText(const char *err, ...);

private:
		TextBox				*m_textBox;
};

#ifdef _DEBUG
#define WPRINTF(wmask, warg) { g_debugWindow->AddMask(wmask); g_debugWindow->AddText warg; }
#else
#define WPRINTF(wmask, warg)
#endif

#endif
