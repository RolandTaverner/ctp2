//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ header
// Description  : 
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
// ACTIVISION_ORIGINAL		
// - When defined, generates the original Activision code.
// - When not defined, generates the modified Apolyton code.
//
// _MSC_VER		
// - Compiler version (for the Microsoft C++ compiler only)
//
// Note: For the blocks with _MSC_VER preprocessor directives, the following
//       is implied: the (_MSC_VER) preprocessor directive lines, and the blocks
//       that are inactive for _MSC_VER value 1200 are modified Apolyton code. 
//       The blocks that are inactiThe blocks that are active for _MSC_VER value 
//       1200 are the original Activision code.
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Reimplemented containers as vectors, to make it less error prone.
//
//----------------------------------------------------------------------------

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#ifndef __STRING_DB_H__
#define __STRING_DB_H__ 1

#if defined(ACTIVISION_ORIGINAL)

class StringRecord;
class Token;

#include "dbtypes.h"


#define STRDB_NUM_HEADS 547

typedef class StringDB { 

    
	
	
	sint32 m_nStr; 
	
	

	
	
	

	StringRecord **m_all; 
	StringRecord **m_head;
	
	

    StringRecord **GetHead(const char *id) const;

public:

	StringDB(); 
    ~StringDB(); 

	sint32 InsertStr(const char *add_id, const char *new_text);   
	sint32 AddStrNode(StringRecord **ptr, const char *add_id, 
					  const char *new_text, StringRecord *&newPtr);
   
	char * GetIdStr(const StringId index) const;
	sint32 GetStringID(char str_id[], StringId &index) const;
	sint32 GetStringID(const char *str_id, StringId &index) const;

	sint32 GetText(const char *get_id, char **new_text); 
	sint32 GetStrNode(StringRecord *ptr, const char *add_id, 
		char **new_text);
   
	void Btree2Array(); 
	void AssignIndex(StringRecord *ptr, StringId &count);
   
	sint32 GetIndexNode(StringRecord *ptr, 
						const char *str_id,
						StringId &index) const;

	const MBCHAR *GetNameStr (StringId n) const;
	const MBCHAR *GetNameStr(const char *s) ;

	sint32 ParseAStringEntry(Token *strToken);
	sint32 Parse(MBCHAR *filename);
} StringDB; 

extern StringDB *g_theStringDB;

#else	// ACTIVISION_ORIGINAL

//----------------------------------------------------------------------------
// Library dependencies
//----------------------------------------------------------------------------

#include <vector>       // std::vector

//----------------------------------------------------------------------------
// Export overview
//----------------------------------------------------------------------------

class	StringDB;

extern	StringDB *	g_theStringDB;

//----------------------------------------------------------------------------
// Project dependencies
//----------------------------------------------------------------------------

#include "c3types.h"	// MBCHAR
#include "dbtypes.h"	// StringId
#include "strrec.h"		// StringRecord
#include "token.h"		// Token

//----------------------------------------------------------------------------
// Class declarations
//----------------------------------------------------------------------------

class StringDB
{
public:
	StringDB(); 
    virtual ~StringDB(); 

	// Modifiers
	bool					InsertStr
	(
		MBCHAR const *			add_id, 
		MBCHAR const *			new_text
	); 
	bool					Parse(MBCHAR * filename);
	
	// Accessors
	MBCHAR *				GetIdStr
	(
		StringId const &		index
	) const;
	MBCHAR const *			GetNameStr(StringId const & n) const;
	MBCHAR const *			GetNameStr(MBCHAR const * s) const;
	bool					GetStringID
	(
		MBCHAR const *			str_id,
		StringId &				index
	) const;
	bool					GetText
	(
		MBCHAR const *			get_id,
		MBCHAR **				new_text
	) const;


private:
	std::vector<StringRecord *>	m_all;	// a flattened list version of m_head
	std::vector<StringRecord *> m_head;	
		// hash vector of B-trees of lexicographically ordered strings

	bool					AddStrNode
	(
		StringRecord * &		ptr,
		MBCHAR const *			add_id,
		MBCHAR const *			new_text,
		StringRecord * &		newPtr
	);
	void					AssignIndex(StringRecord * & ptr);
	void					Btree2Array(void); 
	StringRecord * &		GetHead(MBCHAR const * id);
	StringRecord const * const &	
							GetHead(MBCHAR const * id) const;
	bool					GetIndexNode
	(
		StringRecord const *	ptr,
		MBCHAR const *			str_id,
		StringId &				index
	) const;
	bool					GetStrNode
	(
		StringRecord const *	ptr,
		MBCHAR const *			add_id,
		MBCHAR **				new_text
	) const;

	sint32					ParseAStringEntry(Token * strToken);
};
#endif	// ACTIVISION_ORIGINAL

#endif	// Multiple include guard
