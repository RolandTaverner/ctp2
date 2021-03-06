//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Database substructs record generator header
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
// - Modified AddBitPair function to allow bit pairs to have default values
//   so that when two records are merged, only the bit is merged
//   in that is set. - Sep. 28th 2004 Martin G�hmann
// - Parser for struct ADVANCE_CHANCES of DiffDB.txt can now be generated. (Jan 3rd 2006 Martin G�hmann)
// - Added ParseNum so that a certain number of entries can be parsed if
//   braces are missing so that the old pollution database can be supported. (July 15th 2006 Martin G�hmann)
//
//----------------------------------------------------------------------------

#ifndef __MEMBER_CLASS_H__
#define __MEMBER_CLASS_H__

#include <stdio.h>          // FILE

class MemberClass;

#include "os/include/ctp2_inttypes.h"  // sint32, uint32
#include "gs/dbgen/ctpdb.h"          // DATUM_TYPE, namelist, etc.
#include "ctp/ctp2_utils/pointerlist.h"    // PointerList

class MemberClass
{
public:
	MemberClass(char const * name);
	~MemberClass();

	void AddDatum(DATUM_TYPE type, struct namelist *nameInfo, sint32 minSize, sint32 maxSize,
	              char *subType);
	void AddGroupedBits(char *name, struct namelist *list);
	void AddBitPair(struct namelist *nameInfo, sint32 minSize, sint32 maxSize, struct bitpairtype *pairtype);

	void ExportHeader(FILE *outfile);
	void ExportBits(FILE *outfile);
	void ExportData(FILE *outfile);
	void ExportMethods(FILE *outfile);
	void ExportForwardDeclarations(FILE *outfile);

	void ExportInitialization(FILE *outfile, char *recordName);
	void ExportParser(FILE *outfile, char *recordName);
	void ExportTokenCases(FILE *outfile, char *recordName);
	void ExportDefaultToken(FILE *outfile, char *recordName);
	void ExportOtherRecordIncludes(FILE *outfile);
	void ExportDataCode(FILE *outfile, char *recordName);
	void ExportResolver(FILE *outfile, const char *recordName);

	sint32          FlagCount() const   { return (m_numBits + 31) / 32; };
	const char *    GetName() const     { return m_name; };
	void SetParseNum(sint32 parseNum){ m_parseNum = parseNum; };

private:
	char m_name[k_MAX_RECORD_NAME];
	PointerList<Datum> m_datumList;

	sint32 m_numBits;
	sint32 m_parseNum;
};

#endif
