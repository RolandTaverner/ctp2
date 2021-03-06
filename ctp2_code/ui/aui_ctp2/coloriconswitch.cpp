#include "ctp/c3.h"
#include "ui/aui_ctp2/coloriconswitch.h"

#include "ui/aui_common/aui.h"
#include "ui/aui_ctp2/pattern.h"
#include "ui/aui_ctp2/icon.h"
#include "ui/aui_common/aui_window.h"
#include "ui/aui_common/aui_surface.h"
#include "ui/aui_common/aui_ldl.h"

#include "gfx/gfx_utils/pixelutils.h"
#include "gfx/gfx_utils/colorset.h"       // g_colorSet
#include "gs/fileio/CivPaths.h"       // g_civPaths
#include "ui/aui_utils/primitives.h"
#include "ui/aui_ctp2/c3ui.h"

#include "ui/ldl/ldl_data.hpp"

extern C3UI			*g_c3ui;

ColorIconSwitch::ColorIconSwitch(
	AUI_ERRCODE *retval,
	uint32 id,
	sint32 x,
	sint32 y,
	sint32 width,
	sint32 height,
	MBCHAR *pattern,
	MBCHAR *icon,
	ControlActionCallback *ActionFunc,
	void *cookie )
:
	aui_ImageBase( 1, AUI_IMAGEBASE_BLTTYPE_STRETCH ),
	aui_TextBase(NULL),
	c3_Switch( retval, id, x, y, width, height, pattern, ActionFunc, cookie )
{
	m_shrinkToFit = FALSE;
	m_filename = NULL;

	SetRect(&m_pictureRect, 0, 0, 0, 0);

	InitCommon(icon);
}

ColorIconSwitch::ColorIconSwitch(
	AUI_ERRCODE *retval,
	uint32 id,
	MBCHAR *ldlBlock,
	ControlActionCallback *ActionFunc,
	void *cookie )
	:
	aui_ImageBase( ldlBlock ),
	aui_TextBase(ldlBlock, (MBCHAR *)NULL),
	c3_Switch( retval, id, ldlBlock, ActionFunc, cookie )
{
	m_shrinkToFit = FALSE;
	m_filename = NULL;

	SetRect(&m_pictureRect, 0, 0, 0, 0);

	InitCommon(ldlBlock, TRUE);
}

AUI_ERRCODE ColorIconSwitch::Resize(sint32 width, sint32 height)
{
	AUI_ERRCODE errcode;

	errcode = aui_Switch::Resize(width, height);

	if (errcode == AUI_ERRCODE_OK)
		ResizePictureRect();

	return errcode;
}































void ColorIconSwitch::ResizePictureRect(void)
{
	sint32 destHeight, destWidth;

	if (GetImage(0) && GetImage(0)->TheSurface()) {

		sint32 pictureWidth = GetImage(0)->TheSurface()->Width();
		sint32 pictureHeight = GetImage(0)->TheSurface()->Height();

		RECT buttonRect = {0,0,m_width,m_height};
		RECT iconRect = {0,0,pictureWidth,pictureHeight};

		destHeight = m_height - 2;
		destWidth = m_width - 2;

		if (m_shrinkToFit) {
			double pictureRat = (double)pictureWidth / (double)pictureHeight;

			if (pictureRat < 1.0) {

				iconRect.right =  (sint32)((double)destHeight * pictureRat);
				iconRect.bottom = destHeight;
			} else {

				iconRect.right = destWidth;
				iconRect.bottom = (sint32)((double)destWidth / pictureRat);
			}
		}

		if (buttonRect.right > iconRect.right)  {
			OffsetRect(&iconRect, buttonRect.right/2 - iconRect.right/2, 0);
		} else {
			OffsetRect(&iconRect, iconRect.right/2 - buttonRect.right/2, 0);
		}

		if (buttonRect.bottom > iconRect.bottom) {
			OffsetRect(&iconRect, 0, buttonRect.bottom/2 - iconRect.bottom/2);
		} else {
			OffsetRect(&iconRect, 0, iconRect.bottom/2 - buttonRect.bottom/2);
		}

		m_pictureRect = iconRect;
	}
}

void ColorIconSwitch::SetIcon(MBCHAR *name)
{
	if (name == NULL) return;

	if (m_filename) delete[] m_filename;

	m_filename = new MBCHAR[_MAX_PATH];


	strcpy(m_filename, name);

	if (strcmp(m_filename, ""))
		SetImage(m_filename, 0);










	ResizePictureRect();
}

AUI_ERRCODE ColorIconSwitch::InitCommon( MBCHAR *ldlBlock, BOOL isLDL)
{
	MBCHAR		*name;

	if (isLDL) {
		ldl_datablock * block = aui_Ldl::FindDataBlock(ldlBlock);
		Assert( block != NULL );
		if ( !block ) return AUI_ERRCODE_LDLFINDDATABLOCKFAILED;

		name = block->GetString( "picture" );
		Assert( name != NULL );
	} else {
		name = ldlBlock;
	}

	SetIcon(name);

	return AUI_ERRCODE_OK;
}

ColorIconSwitch::~ColorIconSwitch()
{
	delete [] m_filename;
}

AUI_ERRCODE ColorIconSwitch::DrawThis( aui_Surface *surface, sint32 x, sint32 y )
{

	if ( IsHidden() ) return AUI_ERRCODE_OK;

	if ( !surface ) surface = m_window->TheSurface();

	RECT rect = { 0, 0, m_width, m_height };

	OffsetRect( &rect, m_x + x, m_y + y );
	ToWindow( &rect );

	RECT pictureRect = m_pictureRect;
	OffsetRect(&pictureRect, rect.left, rect.top);

	if (m_pattern)
		m_pattern->Draw( surface, &rect );

	DrawImage(surface, &pictureRect, 0, AUI_IMAGEBASE_SUBSTATE_STATE);

	if ( IsOn() )
	{
		primitives_BevelRect16( surface, &rect, BevelWidth(), 1, 16, 16 );
		RECT temp = rect;
		InflateRect( &temp, -1, -1 );
		primitives_FrameRect16( surface, &temp, g_colorSet->GetColor(COLOR_RED) );
	}
	else
	{
		primitives_BevelRect16( surface, &rect, BevelWidth(), 0, 16, 16 );
	}





	RECT down = rect;
	down.left += 2;
	down.top += 2;

	DrawThisText(
		surface,
		IsOn() ? &down : &rect );

	if ( surface == m_window->TheSurface() )
		m_window->AddDirtyRect( &rect );

	return AUI_ERRCODE_OK;
}
