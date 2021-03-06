//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Multiplayer header
// Id           : $Id$
//
//----------------------------------------------------------------------------
//
// Disclaimer
//
// THIS FILE IS NOT GENERATED OR SUPPORTED BY ACTIVISION.
//
// This material has been developed at apolyton.net by the Apolyton CtP2
// Source Code Project. Contact the authors at ctp2source@apolyton.net.
//
//----------------------------------------------------------------------------
//
// Compiler flags
//
// - None
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Initialized local variables. (Sep 9th 2005 Martin G�hmann)
//
//----------------------------------------------------------------------------

#include "ctp/c3.h"
#include "ui/netshell/ns_header.h"

#include "ui/aui_common/aui_ui.h"
#include "ui/aui_common/aui_uniqueid.h"
#include "ui/netshell/ns_headerswitch.h"
#include "ui/aui_common/aui_ldl.h"

#include "ui/ldl/ldl_file.hpp"

ns_Header::ns_Header(
	AUI_ERRCODE *retval,
	uint32 id,
	MBCHAR *ldlBlock )
:
	aui_ImageBase   (ldlBlock),
	aui_TextBase    (ldlBlock, (MBCHAR const *) NULL),
	aui_Header      (retval, id, ldlBlock)
{
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = aui_Control::InitCommonLdl( ldlBlock, NULL, NULL );
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = aui_SwitchGroup::InitCommonLdl( ldlBlock );
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = aui_Header::InitCommonLdl( ldlBlock );
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = InitCommonLdl( ldlBlock );
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = CreateSwitches( ldlBlock );
	Assert( AUI_SUCCESS(*retval) );
}

ns_Header::ns_Header(
	AUI_ERRCODE *retval,
	uint32 id,
	sint32 x,
	sint32 y,
	sint32 width,
	sint32 height )
:
	aui_ImageBase   ((sint32) 0),
	aui_TextBase    (NULL),
	aui_Header      (retval, id, x, y, width, height)
{
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = aui_Control::InitCommon( NULL, NULL );
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = aui_SwitchGroup::InitCommon();
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = aui_Header::InitCommon();
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = InitCommon();
	Assert( AUI_SUCCESS(*retval) );
	if ( !AUI_SUCCESS(*retval) ) return;

	*retval = CreateSwitches();
	Assert( AUI_SUCCESS(*retval) );
}

AUI_ERRCODE ns_Header::InitCommonLdl( MBCHAR *ldlBlock )
{
	return InitCommon();
}

AUI_ERRCODE ns_Header::InitCommon( void )
{
	return AUI_ERRCODE_OK;
}

AUI_ERRCODE ns_Header::CreateSwitches( MBCHAR *ldlBlock )
{
	AUI_ERRCODE errcode = AUI_ERRCODE_OK;

	static MBCHAR block[ k_AUI_LDL_MAXBLOCK + 1 ];

	if ( ldlBlock )
	{
		sint32 i = 0;
		do
		{
			sprintf( block, "%s.%s%d", ldlBlock, k_AUI_HEADER_LDL_SWITCH, i );

			if ( !aui_Ldl::GetLdl()->FindDataBlock( block ) )
				break;

			aui_Switch *theSwitch = new ns_HeaderSwitch(
				&errcode,
				aui_UniqueId(),
				block );
			Assert( AUI_NEWOK(theSwitch,errcode) );
			if ( !AUI_NEWOK(theSwitch,errcode) )
				return AUI_ERRCODE_MEMALLOCFAILED;

			aui_Action *action = new aui_HeaderSwitchAction( i );
			Assert( action != NULL );
			if ( !action ) return AUI_ERRCODE_MEMALLOCFAILED;

			theSwitch->SetAction( action );

			AddChild( theSwitch );

			i++;

		} while ( 1 );
	}

	return AUI_ERRCODE_OK;
}
