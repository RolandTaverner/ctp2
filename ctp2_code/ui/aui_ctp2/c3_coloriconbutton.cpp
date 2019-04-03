#include "ctp/c3.h"
#include "ui/aui_ctp2/c3_coloriconbutton.h"

#include "ui/aui_common/aui.h"
#include "ui/aui_common/aui_action.h"
#include "ui/aui_ctp2/pattern.h"
#include "ui/aui_ctp2/icon.h"
#include "ui/aui_common/aui_window.h"
#include "ui/aui_common/aui_surface.h"
#include "ui/aui_common/aui_ldl.h"
#include "ui/aui_ctp2/tipwindow.h"

#include "gfx/gfx_utils/pixelutils.h"
#include "gfx/gfx_utils/colorset.h"
#include "gs/fileio/CivPaths.h"

#include "ui/aui_utils/primitives.h"

#include "ui/aui_ctp2/c3ui.h"

#include "ui/ldl/ldl_data.hpp"

extern C3UI			*g_c3ui;
extern CivPaths		*g_civPaths;

c3_ColorIconButton::c3_ColorIconButton(
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
	aui_ImageBase( 1, AUI_IMAGEBASE_BLTTYPE_STRETCH),
	aui_TextBase(NULL),
	c3_Button( retval, id, x, y, width, height, pattern, ActionFunc, cookie )
{
	m_shrinkToFit = FALSE;
	m_filename = NULL;

	SetRect(&m_pictureRect, 0, 0, 0, 0);

	InitCommon(icon);
}

c3_ColorIconButton::c3_ColorIconButton(
	AUI_ERRCODE *retval,
	uint32 id,
	MBCHAR *ldlBlock,
	ControlActionCallback *ActionFunc,
	void *cookie )
	:
	aui_ImageBase( ldlBlock ),
	aui_TextBase(ldlBlock, (MBCHAR *)NULL),
	c3_Button( retval, id, ldlBlock, ActionFunc, cookie )
{
	m_shrinkToFit = FALSE;
	m_filename = NULL;

	SetRect(&m_pictureRect, 0, 0, 0, 0);

	InitCommon(ldlBlock, TRUE);
}

AUI_ERRCODE c3_ColorIconButton::Resize(sint32 width, sint32 height)
{
	AUI_ERRCODE errcode;

	errcode = aui_Button::Resize(width, height);

	if (errcode == AUI_ERRCODE_OK)
		ResizePictureRect();

	return errcode;
}
void c3_ColorIconButton::ResizePictureRect(void)
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

void c3_ColorIconButton::SetIcon(const std::string &name)
{
	if (name.empty()) return;

	if (m_filename) delete[] m_filename;

	m_filename = new MBCHAR[_MAX_PATH];


	strcpy(m_filename, name.c_str());

	if (strcmp(m_filename, ""))
		SetImage(m_filename, 0);
	else
		SetImage(NULL, 0);

	ResizePictureRect();
}

AUI_ERRCODE c3_ColorIconButton::InitCommon( MBCHAR *ldlBlock, BOOL isLDL)
{
  std::string name;

	if (isLDL) {
        ldl_datablock * block = aui_Ldl::FindDataBlock(ldlBlock);
		Assert( block != NULL );
		if ( !block ) return AUI_ERRCODE_LDLFINDDATABLOCKFAILED;

		name = block->GetString( "picture" );
		Assert( !name.empty() );
	} else {
		name = ldlBlock;
	}

	SetIcon(name);

	return AUI_ERRCODE_OK;
}

c3_ColorIconButton::~c3_ColorIconButton()
{
	delete [] m_filename;
}

AUI_ERRCODE c3_ColorIconButton::DrawThis( aui_Surface *surface, sint32 x, sint32 y )
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







	if ( IsDown() )
	{
		primitives_BevelRect16( surface, &rect, 2, 1, 16, 16 );
	}
	else
	{
		primitives_BevelRect16( surface, &rect, 2, 0, 16, 16 );
	}


	pictureRect = rect;
	InflateRect( &pictureRect, -1, -1 );
	DrawImage(surface, &pictureRect, 0, AUI_IMAGEBASE_SUBSTATE_STATE);

	if ( surface == m_window->TheSurface() )
		m_window->AddDirtyRect( &rect );

	return AUI_ERRCODE_OK;
}




void c3_ColorIconButton::MouseLDoubleClickInside( aui_MouseEvent *mouseData )
{
	if (IsDisabled()) return;

	if ( !GetWhichSeesMouse() || GetWhichSeesMouse() == this )
	{
		if ( !GetWhichSeesMouse() ) {

			SetWhichSeesMouse( this );

			ReleaseMouseOwnership();

			PlaySound( AUI_SOUNDBASE_SOUND_EXECUTE );

			m_mouseCode = AUI_ERRCODE_HANDLEDEXCLUSIVE;

			m_attributes &= ~k_CONTROL_ATTRIBUTE_DOWN;




			m_attributes &= ~k_CONTROL_ATTRIBUTE_ACTIVE;

			m_draw |= m_drawMask & k_AUI_REGION_DRAWFLAG_MOUSELDROPINSIDE;

			m_isRepeating = FALSE;

			if ( !HandleGameSpecificLeftClick( this ) )
			if ( m_ActionFunc )
				m_ActionFunc( this, C3_COLORICONBUTTON_ACTION_DOUBLECLK, 0, m_cookie );
			else if ( m_action )
				m_action->Execute( this, C3_COLORICONBUTTON_ACTION_DOUBLECLK, 0 );
		}

		else
		{
			PlaySound( AUI_SOUNDBASE_SOUND_ACTIVATE );

			if ( m_mouseCode == AUI_ERRCODE_UNHANDLED )
				m_mouseCode = AUI_ERRCODE_HANDLED;

			m_attributes |= k_CONTROL_ATTRIBUTE_ACTIVE;
			m_draw |= m_drawMask & k_AUI_REGION_DRAWFLAG_MOUSELDROPINSIDE;
		}
	}
	else
		MouseLDropOutside( mouseData );
}

void c3_ColorIconButton::MouseRGrabInside( aui_MouseEvent *mouseData )
{
	if (IsDisabled()) return;

	if ( !GetWhichSeesMouse() || GetWhichSeesMouse() == this )
	{
		SetWhichSeesMouse( this );

		PlaySound( AUI_SOUNDBASE_SOUND_ENGAGE );

		HideTipWindow();

		SetMouseOwnership();
		SetKeyboardFocus();

		m_mouseCode = AUI_ERRCODE_HANDLEDEXCLUSIVE;

		m_attributes |= k_CONTROL_ATTRIBUTE_DOWN;
		m_attributes |= k_CONTROL_ATTRIBUTE_ACTIVE;
		m_draw |= m_drawMask & k_AUI_REGION_DRAWFLAG_MOUSELGRABINSIDE;

		m_isRepeating = TRUE;
		m_repeatCount = 0;
		m_startWaitTime = mouseData->time;
		if ( m_ActionFunc )
			m_ActionFunc( this, C3_COLORICONBUTTON_ACTION_RIGHT_PRESS, 0, m_cookie );
		else if ( m_action )
			m_action->Execute( this, C3_COLORICONBUTTON_ACTION_RIGHT_PRESS, 0 );
	}
	else
		MouseLGrabOutside( mouseData );
}


void c3_ColorIconButton::MouseRDropInside( aui_MouseEvent *mouseData )
{
	if (IsDisabled()) return;

	if ( !GetWhichSeesMouse() || GetWhichSeesMouse() == this )
	{
		SetWhichSeesMouse( this );

		if ( GetMouseOwnership() == this )
		{

			ReleaseMouseOwnership();

			PlaySound( AUI_SOUNDBASE_SOUND_EXECUTE );

			m_mouseCode = AUI_ERRCODE_HANDLEDEXCLUSIVE;

			m_attributes &= ~k_CONTROL_ATTRIBUTE_DOWN;




			m_attributes &= ~k_CONTROL_ATTRIBUTE_ACTIVE;

			m_draw |= m_drawMask & k_AUI_REGION_DRAWFLAG_MOUSELDROPINSIDE;

			m_isRepeating = FALSE;

			if ( !HandleGameSpecificLeftClick( this ) )
			if ( m_ActionFunc )
				m_ActionFunc( this, C3_COLORICONBUTTON_ACTION_RIGHTCLK, 0, m_cookie );
			else if ( m_action )
				m_action->Execute( this, C3_COLORICONBUTTON_ACTION_RIGHTCLK, 0 );
		}
		else
		{
			PlaySound( AUI_SOUNDBASE_SOUND_ACTIVATE );

			if ( m_mouseCode == AUI_ERRCODE_UNHANDLED )
				m_mouseCode = AUI_ERRCODE_HANDLED;

			m_attributes |= k_CONTROL_ATTRIBUTE_ACTIVE;
			m_draw |= m_drawMask & k_AUI_REGION_DRAWFLAG_MOUSELDROPINSIDE;
		}
	}
	else
		MouseLDropOutside( mouseData );
}


void c3_ColorIconButton::MouseRDropOutside( aui_MouseEvent *mouseData )
{
	if (IsDisabled()) return;

	if ( GetMouseOwnership() == this )
	{

		ReleaseMouseOwnership();

		if ( m_mouseCode == AUI_ERRCODE_UNHANDLED )
			m_mouseCode = AUI_ERRCODE_HANDLED;

		m_draw |= m_drawMask & k_AUI_REGION_DRAWFLAG_MOUSELDROPOUTSIDE;
	}
}
