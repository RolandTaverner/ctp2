#include "ctp/c3.h"
#include "ui/aui_ctp2/picturebutton.h"

#include "ui/aui_common/aui.h"
#include "ui/aui_ctp2/picture.h"
#include "ui/aui_common/aui_window.h"
#include "ui/aui_common/aui_ldl.h"
#include "ui/aui_ctp2/c3ui.h"

#include "ui/ldl/ldl_data.hpp"

extern C3UI *g_c3ui;

PictureButton::PictureButton(
	AUI_ERRCODE *retval,
	uint32 id,
	sint32 x,
	sint32 y,
	sint32 width,
	sint32 height,
	MBCHAR *upPicture,
	MBCHAR *downPicture,
	ControlActionCallback *ActionFunc,
	void *cookie )
:
	aui_ImageBase( (sint32)0 ),
	aui_TextBase(NULL),
	aui_Button      (retval, id, x, y, width, height, ActionFunc, cookie),
    m_upPicture     (NULL),
    m_downPicture   (NULL)
{
	InitCommon(upPicture, downPicture);
}

PictureButton::PictureButton(
	AUI_ERRCODE *retval,
	uint32 id,
	MBCHAR *ldlBlock,
	ControlActionCallback *ActionFunc,
	void *cookie )
:
	aui_ImageBase( ldlBlock ),
	aui_TextBase(ldlBlock, (MBCHAR *)NULL),
	aui_Button      (retval, id, ldlBlock, ActionFunc, cookie),
    m_upPicture     (NULL),
    m_downPicture   (NULL)
{
	InitCommon(ldlBlock, NULL, TRUE);
}

AUI_ERRCODE PictureButton::InitCommon(MBCHAR *upPicture, MBCHAR *downPicture, BOOL isLDL)
{
	MBCHAR		*ldlBlock;
  std::string upName, downName;
	std::string path;

	if (isLDL)
	{
		ldlBlock = upPicture;

		ldl_datablock * block = aui_Ldl::FindDataBlock(ldlBlock);
		Assert( block != NULL );
		if ( !block ) return AUI_ERRCODE_LDLFINDDATABLOCKFAILED;


		upName = block->GetString( "uppicture" );
		Assert( !upName.empty() );
		downName = block->GetString( "downpicture");
		Assert( !downName.empty() );
	} else {
		upName = upPicture;
		downName = downPicture;
	}

	AUI_ERRCODE retval = AUI_ERRCODE_OK;

	delete m_upPicture;
	if (!(path = g_civPaths->FindFile(C3DIR_PICTURES, upName.c_str())).empty()) {
		m_upPicture = new aui_Picture(&retval, path.c_str());
		Assert(retval == AUI_ERRCODE_OK);
	} else {
		m_upPicture = NULL;
	}

	delete m_downPicture;
	if (!(path = g_civPaths->FindFile(C3DIR_PICTURES, downName.c_str())).empty()) {
		m_downPicture = new aui_Picture(&retval, path.c_str());
		Assert(retval == AUI_ERRCODE_OK);
	} else {
		m_downPicture = NULL;
	}

	return AUI_ERRCODE_OK;
}

PictureButton::~PictureButton()
{
	delete m_upPicture;
	delete m_downPicture;
}


AUI_ERRCODE PictureButton::DrawThis( aui_Surface *surface, sint32 x, sint32 y )
{

	if ( IsHidden() ) return AUI_ERRCODE_OK;

	if ( !surface ) surface = m_window->TheSurface();

	RECT rect = { 0, 0, m_width, m_height };
	OffsetRect( &rect, m_x + x, m_y + y );
	ToWindow( &rect );

	if ( IsDown() )
	{
		m_downPicture->Draw( surface, &rect );
	}
	else
	{
		m_upPicture->Draw( surface, &rect );
	}

	if ( surface == m_window->TheSurface() )
		m_window->AddDirtyRect( &rect );

	return AUI_ERRCODE_OK;
}
