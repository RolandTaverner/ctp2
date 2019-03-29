#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef __PICTURESWITCH_H__
#define __PICTURESWITCH_H__

#include "ui/aui_common/aui_switch.h"

class Pattern;
class aui_Picture;

class PictureSwitch : public aui_Switch
{
public:

	PictureSwitch(
		AUI_ERRCODE *retval,
		uint32 id,
		sint32 x,
		sint32 y,
		sint32 width,
		sint32 height,
		aui_Picture *pictureOn = NULL,
		aui_Picture *pictureOff = NULL,
		aui_Picture *pictureActiveOn = NULL,
		aui_Picture *pictureActiveOff = NULL,
		ControlActionCallback *ActionFunc = NULL,
		void *cookie = NULL,
		BOOL selected = FALSE );
	virtual ~PictureSwitch() {}

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
