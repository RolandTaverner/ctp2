#include "ctp/c3.h"
#include "ui/interface/statictextbox.h"

#include "ui/aui_common/aui.h"
#include "ui/aui_common/aui_window.h"
#include "ui/aui_common/aui_ldl.h"

#include "ui/aui_ctp2/c3ui.h"

#include "ui/aui_utils/primitives.h"

#include "ui/aui_utils/textutils.h"

#include "gfx/gfx_utils/colorset.h"

#include "ui/ldl/ldl_data.hpp"

#define k_STATICTEXTBOX_LDL_BEVEL "bevel"

extern C3UI		*g_c3ui;

StaticTextBox::StaticTextBox(
	AUI_ERRCODE *retval,
	uint32 id,
	sint32 x,
	sint32 y,
	sint32 width,
	sint32 height,
	MBCHAR *text,
	uint32 maxLength,
	uint32 size,
	sint32 bevel)
:
	aui_ImageBase( (sint32)0 ),
	aui_TextBase( text, maxLength ),
	aui_Static( retval, id, x, y, width, height )
{
	m_bevel = bevel;
}

StaticTextBox::StaticTextBox(
	AUI_ERRCODE *retval,
	uint32 id,
	MBCHAR *ldlBlock)
	:
	aui_ImageBase(ldlBlock),
	aui_TextBase(ldlBlock, (MBCHAR *)NULL),
	aui_Static(retval, id, ldlBlock)
{
	ldl_datablock * block = aui_Ldl::FindDataBlock(ldlBlock);
	Assert( block != NULL );
	if ( !block ) return;

	if (block->GetAttributeType( k_STATICTEXTBOX_LDL_BEVEL ) == ATTRIBUTE_TYPE_INT) {
		m_bevel = block->GetInt( k_STATICTEXTBOX_LDL_BEVEL );
	} else {
		m_bevel = 1;
	}
}

AUI_ERRCODE StaticTextBox::DrawThis( aui_Surface *surface, sint32 x, sint32 y )
{
	if ( IsHidden() ) return AUI_ERRCODE_OK;

	if ( !surface ) surface = m_window->TheSurface();

	RECT rect = { 0, 0, m_width, m_height };
	OffsetRect( &rect, m_x + x, m_y + y );
	ToWindow( &rect );

	rect.left += 2;

	primitives_BevelRect16( surface, &rect, 1, m_bevel, 16, 16 );

	aui_TextBase::DrawThisText(surface, &rect);

	if ( surface == m_window->TheSurface() )
		m_window->AddDirtyRect( &rect );

	return AUI_ERRCODE_OK;
}
