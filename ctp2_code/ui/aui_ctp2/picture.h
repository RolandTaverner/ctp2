#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef __PICTURE_H__
#define __PICTURE_H__

#include "ui/aui_common/aui_image.h"

class aui_Surface;




class aui_Picture : public aui_Image
{
public:

	aui_Picture(
		AUI_ERRCODE *retval,
		MBCHAR const *szFileName = NULL );
	virtual ~aui_Picture();

	aui_Surface *TheMipmap( void ) const { return m_mipmap; }

	AUI_ERRCODE Draw( aui_Surface *pDestSurf, RECT *pDestRect );

	AUI_ERRCODE MakeMipmap( void );

protected:
	uint16 AveragePixels( uint16 *pBuffer, sint32 width );

	aui_Surface *m_mipmap;
};

#endif
