#include "ctp/c3.h"
#include "ui/aui_ctp2/staticpicture.h"

#include "ui/aui_common/aui.h"
#include "ui/aui_common/aui_ui.h"
#include "ui/aui_common/aui_ldl.h"
#include "ui/aui_common/aui_window.h"
#include "ui/aui_common/aui_blitter.h"

#include "gs/fileio/CivPaths.h"

#include "ui/aui_ctp2/picture.h"
#include "ui/aui_utils/primitives.h"

#include "ui/ldl/ldl_data.hpp"

extern CivPaths		*g_civPaths;
extern aui_UI		*g_ui;

StaticPicture::StaticPicture(
	AUI_ERRCODE *retval,
	uint32 id,
	MBCHAR *ldlBlock,
	MBCHAR *picture )
:
	aui_ImageBase( ldlBlock ),
	aui_TextBase(ldlBlock, (MBCHAR *)NULL),
	aui_Static      (retval, id, ldlBlock),
    m_picture       (NULL)
{
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = InitCommon( ldlBlock, picture );
	Assert( AUI_SUCCESS(*retval) );
}


StaticPicture::StaticPicture(
	AUI_ERRCODE *retval,
	uint32 id,
	sint32 x,
	sint32 y,
	sint32 width,
	sint32 height,
	MBCHAR *picture )
	:
	aui_ImageBase( (sint32)0 ),
	aui_TextBase(NULL),
	aui_Static      (retval, id, x, y, width, height),
    m_picture       (NULL)
{
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = InitCommon( picture );
	Assert( AUI_SUCCESS(*retval) );
}


AUI_ERRCODE StaticPicture::InitCommon( MBCHAR *ldlBlock, MBCHAR *picture )
{
	ldl_datablock * block = aui_Ldl::FindDataBlock(ldlBlock);
	Assert( block != NULL );
	if ( !block ) return AUI_ERRCODE_LDLFINDDATABLOCKFAILED;

  const std::string name =
		block->GetString( picture ? picture : k_AUI_STATICIMAGE_LDL_IMAGE );
	Assert( !name.empty());
	if ( !name.empty() )
	{
		AUI_ERRCODE errcode = InitCommon( name.c_str() );
		Assert( AUI_SUCCESS(errcode) );
		return errcode;
	}

	return AUI_ERRCODE_OK;
}


AUI_ERRCODE StaticPicture::InitCommon( const MBCHAR *picture )
{
	const std::string filename = g_civPaths->FindFile(C3DIR_PICTURES, picture);
	if (!filename.empty()) {
	  AUI_ERRCODE errcode;
		m_picture = new aui_Picture(&errcode, filename.c_str());
	} else {
    m_picture = NULL;
	}

	Assert( m_picture != NULL );
	return AUI_ERRCODE_OK;
}


StaticPicture::~StaticPicture()
{
	delete m_picture;
}


AUI_ERRCODE StaticPicture::DrawThis( aui_Surface *surface, sint32 x, sint32 y )
{

	if ( IsHidden() ) return AUI_ERRCODE_OK;

	if ( !surface ) surface = m_window->TheSurface();

	RECT rect = { 0, 0, m_width, m_height };

	OffsetRect( &rect, m_x + x, m_y + y );
	ToWindow( &rect );

	primitives_BevelRect16( surface, &rect, 2, 1, 16, 16);
	InflateRect( &rect, -2, -2 );

	if ( m_picture )
		m_picture->Draw( surface, &rect );

	if ( surface == m_window->TheSurface() )
		m_window->AddDirtyRect( &rect );

	return AUI_ERRCODE_OK;
}

void StaticPicture::SetPicture(const MBCHAR *picture)
{
  delete m_picture;
  const std::string filename = g_civPaths->FindFile(C3DIR_PICTURES, picture);
	if (!filename.empty()) {
	  AUI_ERRCODE errcode;
		m_picture = new aui_Picture(&errcode, filename.c_str());
	} else {
		m_picture = NULL;
	}

	RECT rect = { 0, 0, m_width, m_height };
	ToWindow(&rect);
	m_window->AddDirtyRect(&rect);
}
