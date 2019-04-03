#include "ctp/c3.h"

#include "ui/aui_common/aui_ui.h"
#include "ui/aui_common/aui_ldl.h"
#include "ui/ldl/ldl_data.hpp"
#include "ui/netshell/ns_string.h"

#include "gs/database/StrDB.h"

extern StringDB	*g_theStringDB;

ns_String::ns_String( char *ldlBlock )
:
    m_string    (NULL)
{
    ldl_datablock * block = aui_Ldl::FindDataBlock(ldlBlock);
	Assert( block != NULL );
	if ( !block ) return;

	std::string string;

	if ( block->GetBool(k_NS_STRING_LDL_NODATABASE) || block->GetString("text").empty()) {

		string = block->GetString( "text" );
	}
	else {
		string = g_theStringDB->GetNameStr( block->GetString("text").c_str() );
	}

	m_string = new char[string.length() + 1];
    strcpy(m_string, string.c_str());
}

ns_String::~ns_String( void )
{
	delete [] m_string;
}

char *ns_String::GetString( void ) {
	return m_string;
}
