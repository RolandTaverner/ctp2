#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef __PICTURETAB_H__
#define __PICTURETAB_H__

#include "ui/aui_common/aui_tab.h"

class aui_Picture;

class PictureTab : public aui_Tab
{
public:

	PictureTab(
		AUI_ERRCODE *retval,
		uint32 id,
		sint32 x,
		sint32 y,
		sint32 width,
		sint32 height,
		sint32 paneWidth,
		sint32 paneHeight,
		aui_Picture *pictureOn,
		aui_Picture *pictureOff,
		aui_Picture *pictureActiveOn,
		aui_Picture *pictureActiveOff,
		ControlActionCallback *ActionFunc = NULL,
		void *cookie = NULL,
		BOOL selected = FALSE );
	virtual ~PictureTab() {}

	aui_Picture *&ThePictureOn( void ) { return m_pictureOn; }
	aui_Picture *&ThePictureOff( void ) { return m_pictureOff; }
	aui_Picture *&ThePictureActiveOn( void ) { return m_pictureActiveOn; }
	aui_Picture *&ThePictureActiveOff( void ) { return m_pictureActiveOff; }

	virtual AUI_ERRCODE DrawThis(
		aui_Surface *surface = NULL,
		sint32 x = 0,
		sint32 y = 0 );

protected:
	aui_Picture *m_pictureOn;
	aui_Picture *m_pictureOff;
	aui_Picture *m_pictureActiveOn;
	aui_Picture *m_pictureActiveOff;
};

#endif
