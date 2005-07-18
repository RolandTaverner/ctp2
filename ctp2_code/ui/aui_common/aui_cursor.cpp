










#include "c3.h"

#include "aui_Factory.h"
#include "aui_cursor.h"



aui_Cursor::aui_Cursor(
	AUI_ERRCODE *retval,
	const MBCHAR *filename )
	:
	aui_Image( retval, filename )
{
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = InitCommon();
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;
}



AUI_ERRCODE aui_Cursor::InitCommon( void )
{
	memset( &m_hotspot, 0, sizeof( m_hotspot ) );

	return AUI_ERRCODE_OK;
}



AUI_ERRCODE aui_Cursor::GetHotspot( sint32 *x, sint32 *y )
{
	if ( x ) *x = m_hotspot.x;
	if ( y ) *y = m_hotspot.y;

	return AUI_ERRCODE_OK;
}



AUI_ERRCODE aui_Cursor::SetHotspot( sint32 x, sint32 y )
{
	m_hotspot.x = x;
	m_hotspot.y = y;

	return AUI_ERRCODE_OK;
}




AUI_ERRCODE aui_Cursor::LoadEmpty( sint32 width, sint32 height, sint32 bpp )
{
	AUI_ERRCODE retcode;

	m_surface = aui_Factory::new_Surface(retcode, width, height, bpp);

	Assert( AUI_NEWOK(m_surface,retcode) );
	return retcode;
}

