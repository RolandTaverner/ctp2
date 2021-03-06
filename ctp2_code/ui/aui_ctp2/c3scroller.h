#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif

#ifndef __C3SCROLLER_H__
#define __C3SCROLLER_H__

class C3Scroller;

#include "ui/aui_common/aui_control.h"    // ControlActionCallback
#include "ui/aui_common/aui_ranger.h"
#include "os/include/ctp2_inttypes.h"  // sint32, uint32
#include "ui/aui_ctp2/patternbase.h"

class aui_Surface;

class C3Scroller : public aui_Ranger, public PatternBase
{
public:
	C3Scroller
    (
		AUI_ERRCODE *retval,
		uint32 id,
		MBCHAR *ldlBlock,
		ControlActionCallback *ActionFunc = NULL,
		void *cookie = NULL
    );
	C3Scroller
    (
		AUI_ERRCODE *retval,
		uint32 id,
		sint32 x,
		sint32 y,
		sint32 width,
		sint32 height,
		bool   isVertical,
		MBCHAR *pattern,
		ControlActionCallback *ActionFunc = NULL,
		void *cookie = NULL
    );

	virtual AUI_ERRCODE DrawThis
    (
		aui_Surface *surface = NULL,
		sint32 x = 0,
		sint32 y = 0
    );

	bool	IsVertical( void ) const { return m_isVertical; }

private:
	AUI_ERRCODE CreateButtonsAndThumb( void );
	AUI_ERRCODE InitCommon(void);

	bool	m_isVertical;
};

#endif
