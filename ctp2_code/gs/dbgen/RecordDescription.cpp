//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Database record generator
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
// WIN32
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Implemented GovernmentsModified subclass (allowing cdb files including
//   a GovernmentsModified record to produce parsers capable of reading and
//   storing subrecords for Government types.)
//   See http://apolyton.net/forums/showthread.php?s=&threadid=107916 for
//   more details  _____ by MrBaggins Jan-04
//
//   * Added m_hasGovernmentsModified (bool,) used throughout to incorporate
//     appropriate code for each parser
//   * Code dynamically generates m_hasGovernmentsModified into the record 
//     parser, and sets it, accordingly.
//   * Code dynamically generates (along with a public accessor for 
//     m_hasGovernmentsModified,) generic accessors...
//     - sint32 GenericGetNumGovernmentsModified()
//     - sint32 GenericGetGovernmentsModifiedIndex(sint32 index)
//     because a template requires access to these properties which may or
//     may not exist in the templated class.  These functions return an
//     appropriate value in those cases.
//   * Code generates a conditional parser construct to deal with an
//     addition to the standard parsed record syntax.
//
// - Added return type void to Init function to make .NET quiet about the
//   missing return type, by Martin G�hmann.
// - Modified AddBitPair function to allow bit pairs to have default values
//   so that when two records are merged, only the bit is merged 
//   in that is set. - Sep. 28th 2004 Martin G�hmann
// - Added serilization method export. (Aug 24th 2005 Martin G�hmann)
// - Output files only have spaces instead of tabs as indent and indetion
//   was fixed. (Aug 25th 2005 Martin G�hmann)
// - Added alias names and the possibility to have default values from 
//   other entries. (Aug 26th 2005 Martin G�hmann)
// - Added accessors for slic database array access. (Sep 16th 2005 Martin G�hman)
// - Made float arrays possible. (Sep 16th 2005 Martin G�hman)
// - Made value of int databases accessable. (Sep 16th 2005 Martin G�hman)
// - If database records have no name a default name is generated. e.g.
//   DIFFICULTY_5 for the sixth entry in the DifficultyDB. (Jan 3rd 2006 Martin G�hman)
//
//----------------------------------------------------------------------------
#include "ctp2_config.h"
#include "ctp2_inttypes.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(WIN32)
#include <windows.h>
#else
#include "windows.h"
#endif
#include "ctpdb.h"
#include "RecordDescription.h"

extern "C" int g_generateRequirementWarnings;

RecordDescription::RecordDescription(char *name)
:
	m_datumList                 (),
	m_memberClasses             (),
	m_hasGovernmentsModified    (false),
	m_numBits                   (0),
	m_addingToMemberClass       (false),
	m_baseType                  (DATUM_NONE)
{
	strncpy(m_name, name, k_MAX_RECORD_NAME);
}

RecordDescription::~RecordDescription()
{
}

void RecordDescription::SetBaseType(DATUM_TYPE type)
{ 
	char *name = (char *)malloc(strlen("Value")  + 1);
	strcpy(name, "Value");

	m_baseType = type;
	Datum *dat = new Datum;
	dat->m_type = type;
	dat->m_name = name;
	dat->m_minSize = -1;
	dat->m_maxSize = -1;
	dat->m_subType = NULL;
	dat->m_groupList = NULL;
	dat->m_bitPairDatum = NULL;
	dat->m_required = true;

	m_datumList.AddTail(dat);

	name = (char *)malloc(strlen("NameText")  + 1);
	strcpy(name, "NameText");

	dat = new Datum;
	dat->m_type = DATUM_STRING;
	dat->m_name = name;
	dat->m_minSize = -1;
	dat->m_maxSize = -1;
	dat->m_subType = NULL;
	dat->m_groupList = NULL;
	dat->m_bitPairDatum = NULL;

	m_datumList.AddTail(dat);
}

void RecordDescription::ExportHeader(FILE *outfile)
{
	fprintf(outfile, "#ifndef __%s_RECORD_H__\n#define __%s_RECORD_H__\n\n",
			m_name, m_name);

	fprintf(outfile, "/*\n * %s description\n */\n", m_name);
	fprintf(outfile, "#include \"CTPRecord.h\"\n");

	ExportBits(outfile);
	ExportRanges(outfile);

	fprintf(outfile, "\n/*\n");
	fprintf(outfile, " * DO NOT EDIT THIS FILE!\n");
	fprintf(outfile, " * It is generated automatically by ctpdb\n");
	fprintf(outfile, " */\n");

	ExportForwardDeclarations(outfile);

	fprintf(outfile, "\nclass %sRecord : public CTPRecord\n{\npublic:\n", m_name);
	
	fprintf(outfile, "    typedef sint32 (%sRecord::*IntAccessor)() const;\n", m_name);
	fprintf(outfile, "    typedef bool   (%sRecord::*BoolAccessor)() const;\n", m_name);
	fprintf(outfile, "    typedef double (%sRecord::*FloatAccessor)() const ;\n", m_name);
	fprintf(outfile, "    typedef bool   (%sRecord::*BitIntAccessor)(sint32 &val) const;\n", m_name);
	fprintf(outfile, "    typedef bool   (%sRecord::*BitFloatAccessor)(double &val) const;\n", m_name);
	fprintf(outfile, "    typedef sint32 (%sRecord::*IntArrayAccessor)(sint32 index) const;\n", m_name);
	fprintf(outfile, "    typedef double (%sRecord::*FloatArrayAccessor)(sint32 index) const ;\n", m_name);

	ExportMemberClasses(outfile);
	fprintf(outfile, "private:\n");
	ExportData(outfile);

	fprintf(outfile, "//GovMod Specific flag\n");
	fprintf(outfile, "    bool m_hasGovernmentsModified;\n\n");
	
	fprintf(outfile, "\npublic:\n");

	ExportMethods(outfile);

	fprintf(outfile, "    //GovMod Specific accessors\n");
	fprintf(outfile, "    bool GetHasGovernmentsModified() const { return m_hasGovernmentsModified; }\n\n");
	fprintf(outfile, "");
	fprintf(outfile, "");
	fprintf(outfile, "    sint32 GenericGetNumGovernmentsModified() {");
	
	if(!m_hasGovernmentsModified) 
		fprintf(outfile," return 0; }\n\n\n");
	else
		fprintf(outfile," return m_numGovernmentsModified; }\n\n\n");

	fprintf(outfile, "    sint32 GenericGetGovernmentsModifiedIndex(sint32 index) {");
	if(!m_hasGovernmentsModified) 
		fprintf(outfile," return -1; }\n\n\n");
	else
		fprintf(outfile," return GetGovernmentsModifiedIndex(index); }\n\n\n");

  
	fprintf(outfile, "}; /* %sRecord */\n\n", m_name);
	fprintf(outfile, "struct %sRecordAccessorInfo {\n", m_name);
	fprintf(outfile, "    %sRecord::IntAccessor m_intAccessor;\n", m_name);
	fprintf(outfile, "    %sRecord::BoolAccessor m_boolAccessor;\n", m_name);
	fprintf(outfile, "    %sRecord::FloatAccessor m_floatAccessor;\n", m_name);
	fprintf(outfile, "    %sRecord::BitIntAccessor m_bitIntAccessor;\n", m_name);
	fprintf(outfile, "    %sRecord::BitFloatAccessor m_bitFloatAccessor;\n", m_name);
	fprintf(outfile, "    %sRecord::IntArrayAccessor m_intArrayAccessor;\n", m_name);
	fprintf(outfile, "    %sRecord::FloatArrayAccessor m_floatArrayAccessor;\n", m_name);
	fprintf(outfile, "};\n");

	fprintf(outfile, "extern %sRecordAccessorInfo g_%sRecord_Accessors[];\n", m_name, m_name);
	fprintf(outfile, "extern char *g_%s_Tokens[];\n", m_name);

	sint32 count = 0;
	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		count++;
		if(walk.GetObj()->m_type == DATUM_BIT_PAIR)
			count++;
		walk.Next();
	}

	fprintf(outfile, "#define k_Num_%sRecord_Tokens %d\n\n", m_name, count);

	fprintf(outfile, "template <class T> class CTPDatabase;\n");
	fprintf(outfile, "extern CTPDatabase<%sRecord> *g_the%sDB;\n\n", m_name, m_name);

	fprintf(outfile, "#endif /* ifndef __%s_RECORD_H__ */\n", m_name);
}

void RecordDescription::ExportForwardDeclarations(FILE *outfile)
{
	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		if(walk.GetObj()->m_type == DATUM_RECORD) {
			fprintf(outfile, "class %sRecord;\n", walk.GetObj()->m_subType);
		}
		if(walk.GetObj()->m_type == DATUM_BIT_PAIR &&
			(DATUM_TYPE)walk.GetObj()->m_bitPairDatum->m_type == DATUM_RECORD) {
			fprintf(outfile, "class %sRecord;\n", walk.GetObj()->m_bitPairDatum->m_subType);
		}
		walk.Next();
	}

	PointerList<MemberClass>::Walker membWalk(&m_memberClasses);
	while(membWalk.IsValid()) {
		membWalk.GetObj()->ExportForwardDeclarations(outfile);
		membWalk.Next();
	}
}



void RecordDescription::ExportDataCode(FILE *outfile)
{
	fprintf(outfile, "/*\n * Data accessors\n * (DO NOT EDIT!  Automatically generated file)\n */\n");

	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		walk.GetObj()->ExportDataCode(outfile, m_name);
		walk.Next();
	}
}

void RecordDescription::AddDatum(DATUM_TYPE type, struct namelist *nameInfo,
                                 sint32 minSize, sint32 maxSize,
                                 char *subType)
{
	if(m_addingToMemberClass) {
		
		Assert(m_memberClasses.GetTail());
		if(m_memberClasses.GetTail()) {
			m_memberClasses.GetTail()->AddDatum(type, nameInfo,
												minSize, maxSize,
												subType);
		}
		return;
	}
	
	Datum *dat = new Datum;
	dat->m_type = type;
	dat->m_name = nameInfo->name;
	dat->m_akaName = nameInfo->akaName;
	dat->m_defaultName = nameInfo->defaultName;
	dat->m_minSize = minSize;
	dat->m_maxSize = maxSize;
	dat->m_subType = subType;
	dat->m_groupList = NULL;
	dat->m_bitPairDatum = NULL;
	if((!(nameInfo->flags & k_NAMEVALUE_HAS_VALUE)) &&
	   (dat->m_maxSize <= 0)) {
		switch(dat->m_type) {
			default:
				break;
			case DATUM_INT:
			case DATUM_FLOAT:
			case DATUM_STRUCT:
			case DATUM_STRING:
			case DATUM_FILE:
			case DATUM_STRINGID:
				dat->m_required = true;
				break;
		}
	} else {
		dat->SetValue(nameInfo->v);
	}

	m_datumList.AddTail(dat);

	if(dat->m_type == DATUM_BIT) {
		
		dat->m_bitNum = m_numBits;
		m_numBits++;
	} else {
		dat->m_bitNum = -1;
	}
}

void RecordDescription::AddGroupedBits(char *groupName, struct namelist *list)
{
	if(m_addingToMemberClass) {
		Assert(m_memberClasses.GetTail());
		if(m_memberClasses.GetTail()) {
			m_memberClasses.GetTail()->AddGroupedBits(groupName, list);
		}
		return;
	}
	
	
	Datum *dat = new Datum;
	dat->m_type = DATUM_BIT_GROUP;
	dat->m_name = groupName;
	dat->m_minSize = -1;
	dat->m_maxSize = -1;
	dat->m_subType = NULL;
	dat->m_groupList = list;
	dat->m_bitPairDatum = NULL;
	m_datumList.AddTail(dat);
}

// Added by Martin G�hmann
void RecordDescription::AddBitPair(struct namelist *nameInfo, sint32 minSize, sint32 maxSize, struct bitpairtype *pairtype)
{
	if(m_addingToMemberClass) {
		Assert(m_memberClasses.GetTail());
		if(m_memberClasses.GetTail()) {
// Added by Martin G�hmann
			m_memberClasses.GetTail()->AddBitPair(nameInfo, minSize, maxSize, pairtype);
		}
		return;
	}


	Datum *dat = new Datum;
	dat->m_type = DATUM_BIT_PAIR;
// Added by Martin G�hmann
	dat->m_name = nameInfo->name;
	dat->m_minSize = minSize;
	dat->m_maxSize = maxSize;
	dat->m_subType = NULL;
	dat->m_groupList = NULL;
// Added by Martin G�hmann for adding default values
	if((nameInfo->flags & k_NAMEVALUE_HAS_VALUE)
	|| (dat->m_maxSize > 0)
	){
		dat->SetValue(nameInfo->v);
	}
	dat->m_bitNum = m_numBits;
	m_numBits++;

	Datum *pairDat = new Datum;
	dat->m_bitPairDatum = pairDat;
	pairDat->m_type = (DATUM_TYPE)pairtype->type;
	pairDat->m_name = (char *)malloc(strlen(dat->m_name) + strlen("Value") + 1);
	strcpy(pairDat->m_name, dat->m_name);
	strcat(pairDat->m_name, "Value");
	pairDat->m_minSize = -1;
	pairDat->m_maxSize = -1;
	pairDat->m_subType = (char *)pairtype->extraData;
	pairDat->m_groupList = NULL;
	pairDat->m_bitPairDatum = NULL;

	m_datumList.AddTail(dat);
}


void RecordDescription::StartMemberClass(char *name)
{
	
	m_memberClasses.AddTail(new MemberClass(name));
	m_addingToMemberClass = true;
}

void RecordDescription::EndMemberClass(char *name)
{
	
	Assert(m_memberClasses.GetTail());
	if(m_memberClasses.GetTail()) {
		Assert(!strcmp(m_memberClasses.GetTail()->GetName(), name));
	}
	m_addingToMemberClass = false;
}

void RecordDescription::ExportBits(FILE *outfile)
{
	sint32 bit = 0;
	char nicename[k_MAX_RECORD_NAME];
	
	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		if(dat->m_type == DATUM_BIT || dat->m_type == DATUM_BIT_PAIR) {
			if(!(bit % 32)) {
				fprintf(outfile, "//\n// m_flags%d: %s\n", bit / 32, m_name);
			}
			sprintf(nicename, "k_%s_%s_Bit", m_name, dat->m_name);
			fprintf(outfile, "#define %-40s 0x%08lx\n", nicename, 1 << (bit % 32));
			bit++;
		}
		walk.Next();
	}
	
	walk.SetList(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		if(dat->m_type == DATUM_BIT_GROUP) {
			sint32 bit = 0;
			fprintf(outfile, "//\n// m_%s bit group\n", dat->m_name);
			struct namelist *node = dat->m_groupList;
			while(node) {
				sprintf(nicename, "k_%s_%s_%s_Bit", m_name, dat->m_name, node->name);
				fprintf(outfile, "#define %-40s 0x%08lx\n", nicename, 1 << bit);
				bit++;
				Assert(bit <= 32);
				node = node->next;
			}
		}
		walk.Next();
	}

	PointerList<MemberClass>::Walker membWalk(&m_memberClasses);
	while(membWalk.IsValid()) {
		membWalk.GetObj()->ExportBits(outfile);
		membWalk.Next();
	}
}

void RecordDescription::ExportRanges(FILE *outfile)
{
	
	fprintf(outfile, "//\n");
	fprintf(outfile, "// Range definitions\n");

	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		walk.GetObj()->ExportRangeDefines(outfile);
		walk.Next();
	}
}

void RecordDescription::ExportData(FILE *outfile)
{
	if(m_numBits > 0) {
		
		sint32 flag;
		for(flag = 0; flag <= m_numBits / 32; flag++) {
			fprintf(outfile, "    uint32 m_flags%d;\n", flag);
		}
		fprintf(outfile, "\n");
	}

	
	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		if(walk.GetObj()->m_type != DATUM_BIT) {
			walk.GetObj()->ExportVariable(outfile, 0);
		}
		walk.Next();
	}
}

void RecordDescription::ExportMethods(FILE *outfile)
{
	
	fprintf(outfile, "    %sRecord() { Init(); };\n", m_name);
	fprintf(outfile, "    %sRecord(CivArchive &archive) { Serialize(archive); };\n", m_name);
	fprintf(outfile, "    ~%sRecord();\n", m_name);

	//Added by Martin G�hmann functions needs a return type
	fprintf(outfile, "    void Init();\n", m_name);
	fprintf(outfile, "    void Serialize(CivArchive &archive);\n\n", m_name);

	fprintf(outfile, "    // These methods are needed for records to conform to\n");
	fprintf(outfile, "    // 'Orthodox Cannonical Form' and work with resizing STL vectors.\n");
	fprintf(outfile, "    %sRecord(const %sRecord &rval) { Init(); *this = rval; }\n", m_name, m_name);
	fprintf(outfile, "    void operator=(const %sRecord &rval);\n\n", m_name);
	








	fprintf(outfile, "    void CheckRequiredFields(DBLexer *lex);\n");
	fprintf(outfile, "    sint32 Parse(DBLexer *lex, sint32 numRecords);\n\n");
	fprintf(outfile, "    void ResolveDBReferences();\n");
	fprintf(outfile, "    void Merge(const %sRecord & rval);\n", m_name);

	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		if(walk.GetObj()->m_type == DATUM_BIT_GROUP) {
			fprintf(outfile, "    sint32 Parse%sBit(DBLexer *lex);\n", walk.GetObj()->m_name);
		}
		walk.Next();
	}

	fprintf(outfile, "    //\n    // Accessors\n    //\n");
	
	walk.SetList(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();

		if(strcmp("GovernmentsModified", walk.GetObj()->m_name)==0) m_hasGovernmentsModified=true;

		dat->ExportAccessor(outfile, 0, m_name);
		walk.Next();
	}
}

void RecordDescription::ExportMemberClasses(FILE *outfile)
{
	
	PointerList<MemberClass>::Walker walk(&m_memberClasses);
	while(walk.IsValid()) {
		MemberClass *mem = walk.GetObj();
		mem->ExportHeader(outfile);
		walk.Next();
	}
}
	
void RecordDescription::ExportCode(FILE *outfile)
{
	
	fprintf(outfile, "#include \"c3.h\"\n");
	fprintf(outfile, "#include \"c3errors.h\"\n");
	fprintf(outfile, "#include \"%sRecord.h\"\n", m_name);
	fprintf(outfile, "#include \"DBLexer.h\"\n");
	fprintf(outfile, "#include \"DBTokens.h\"\n");
	fprintf(outfile, "#include \"CTPDatabase.h\"\n");
	fprintf(outfile, "#include \"StrDB.h\"\n");
	fprintf(outfile, "#include \"BitArray.h\"\n");
	fprintf(outfile, "\n");

	ExportOtherRecordIncludes(outfile);
	fprintf(outfile, "\n");

	fprintf(outfile, "CTPDatabase<%sRecord> *g_the%sDB = NULL;\n", m_name, m_name);
	fprintf(outfile, "\n");
	ExportManagement(outfile);
	ExportParser(outfile);
	ExportMerger(outfile);
	ExportDataCode(outfile);
	ExportMemberClassDataCode(outfile);
}

void RecordDescription::ExportOtherRecordIncludes(FILE *outfile)
{
	
	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		if(walk.GetObj()->m_type == DATUM_RECORD) {
			
			
			fprintf(outfile, "#include \"%sRecord.h\"\n", walk.GetObj()->m_subType);
		}
		if(walk.GetObj()->m_type == DATUM_BIT_PAIR &&
			(DATUM_TYPE)walk.GetObj()->m_bitPairDatum->m_type == DATUM_RECORD) {
			fprintf(outfile, "#include \"%sRecord.h\"\n", walk.GetObj()->m_bitPairDatum->m_subType);
		}
		walk.Next();
	}

	PointerList<MemberClass>::Walker membWalk(&m_memberClasses);
	while(membWalk.IsValid()) {
		membWalk.GetObj()->ExportOtherRecordIncludes(outfile);
		membWalk.Next();
	}
}

void RecordDescription::ExportManagement(FILE *outfile)
{

	//Added by Martin G�hmann function need a return type
	fprintf(outfile, "void %sRecord::Init()\n", m_name);
	fprintf(outfile, "{\n");
	
	sint32 i;
	for(i = 0; i  < ((m_numBits + 31)/ 32); i++) {
		fprintf(outfile, "    m_flags%d = 0;\n", i);
	}

	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		dat->ExportInitialization(outfile);
		walk.Next();
	}
	
	fprintf(outfile, "    //GovMod Specific flag initialization\n");
	if (m_hasGovernmentsModified)
		fprintf(outfile, "    m_hasGovernmentsModified=true;\n\n");
	else
		fprintf(outfile, "    m_hasGovernmentsModified=false;\n\n");

	fprintf(outfile, "}\n\n");


	fprintf(outfile, "void %sRecord::Serialize(CivArchive &archive)\n", m_name);
	fprintf(outfile, "{\n");

	fprintf(outfile, "    if(archive.IsStoring()) {\n");

	fprintf(outfile, "        archive << m_index;\n");
	fprintf(outfile, "        if(m_name >= 0){\n");
	fprintf(outfile, "            archive << GetIDText();\n");
	fprintf(outfile, "        }\n");
	fprintf(outfile, "        else{\n");
	fprintf(outfile, "            archive << static_cast<MBCHAR*>(NULL);\n");
	fprintf(outfile, "        }\n");
	for(i = 0; i  < ((m_numBits + 31)/ 32); i++) {
		fprintf(outfile, "        archive << m_flags%d;\n", i);
	}

	walk.SetList(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		dat->ExportSerializationStoring(outfile);
		walk.Next();
	}

	fprintf(outfile, "    } else {\n");

	fprintf(outfile, "        //GovMod Specific flag initialization\n");
	if (m_hasGovernmentsModified)
		fprintf(outfile, "        m_hasGovernmentsModified=true;\n\n");
	else
		fprintf(outfile, "        m_hasGovernmentsModified=false;\n\n");


	fprintf(outfile, "        archive >> m_index;\n");
	fprintf(outfile, "        {\n");
	fprintf(outfile, "            MBCHAR* tmpStr = NULL;\n");
	fprintf(outfile, "            archive >> tmpStr;\n");
	fprintf(outfile, "            g_theStringDB->GetStringID(tmpStr, m_name);\n");
	fprintf(outfile, "            SetTextName(g_theStringDB->GetNameStr(m_name));\n");
	fprintf(outfile, "        }\n");
	for(i = 0; i  < ((m_numBits + 31)/ 32); i++) {
		fprintf(outfile, "        archive >> m_flags%d;\n", i);
	}

	walk.SetList(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		dat->ExportSerializationLoading(outfile);
		walk.Next();
	}

	fprintf(outfile, "    }\n");
	fprintf(outfile, "}\n\n");


	fprintf(outfile, "%sRecord::~%sRecord()\n", m_name, m_name);
	fprintf(outfile, "{\n");
	fprintf(outfile, "    int index = 0;\n");
	walk.SetList(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		dat->ExportDestructor(outfile);
		walk.Next();
	}
	fprintf(outfile, "}\n\n");
	

	fprintf(outfile, "void %sRecord::operator=(const %sRecord & rval)\n", m_name, m_name);
	fprintf(outfile, "{\n");
	fprintf(outfile, "    int index = 0;\n");
	
	fprintf(outfile, "    m_index = rval.m_index;\n");

	walk.SetList(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		dat->ExportOperatorEqual(outfile);
		walk.Next();
	}
	fprintf(outfile, "}\n\n");
}

void RecordDescription::ExportMerger(FILE *outfile)
{
	fprintf(outfile, "void %sRecord::Merge(const %sRecord & rval)",
		m_name, m_name);
	fprintf(outfile, "{\n");
	PointerList<Datum>::Walker walk(&m_datumList);
	fprintf(outfile, "    int index = 0, obj_index = 0;\n");
	while(walk.IsValid()) {
		if(walk.GetObj()->m_type == DATUM_BIT_PAIR || 
		   walk.GetObj()->m_maxSize >= 0 ||
		   walk.GetObj()->m_type == DATUM_BIT_GROUP) {
			walk.GetObj()->ExportMerge(outfile, m_name);
		}
		walk.Next();
	}
	fprintf(outfile, "}\n\n");

}

void RecordDescription::ExportParser(FILE *outfile)
{

	char nicename[k_MAX_STRING];

	fprintf(outfile, "char *g_%s_Tokens[] =\n", m_name);
	fprintf(outfile, "{\n");
	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		fprintf(outfile, "    \"%s\",\n", walk.GetObj()->m_name);
		if(walk.GetObj()->m_type == DATUM_BIT_PAIR) {
			fprintf(outfile, "    \"%s\",\n", walk.GetObj()->m_bitPairDatum->m_name);
		}
		walk.Next();
	}
	walk.SetList(&m_datumList);
	while(walk.IsValid()) {
		if(walk.GetObj()->m_akaName){
			fprintf(outfile, "    \"%s\",\n", walk.GetObj()->m_akaName);
		}
		walk.Next();
	}

	fprintf(outfile, "};\n\n");
	
	fprintf(outfile, "%sRecordAccessorInfo g_%sRecord_Accessors[] =\n", m_name, m_name);
	fprintf(outfile, "{\n");
	walk.SetList(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		switch(dat->m_type) {
			case DATUM_INT:
				if(dat->m_maxSize < 0) {
					fprintf(outfile, "    { &%sRecord::Get%s, NULL, NULL, NULL, NULL, NULL, NULL },\n", m_name, dat->m_name);
				} else {
					fprintf(outfile, "    { &%sRecord::GetNum%s, NULL, NULL, NULL, NULL, &%sRecord::Get%s, NULL}, /* %s (array) */\n", m_name, dat->m_name, m_name, dat->m_name, dat->m_name);
				}
				break;
			case DATUM_RECORD:
				if(dat->m_maxSize < 0) {
					fprintf(outfile, "    { &%sRecord::Get%sIndex, NULL, NULL, NULL, NULL, NULL, NULL },\n", m_name, dat->m_name);
				} else {
					fprintf(outfile, "    { &%sRecord::GetNum%s, NULL, NULL, NULL, NULL, &%sRecord::Get%sIndex, NULL}, /* %s (array) */\n", m_name, dat->m_name, m_name, dat->m_name, dat->m_name);
				}
				break;
			case DATUM_BIT:
				fprintf(outfile, "    { NULL, &%sRecord::Get%s, NULL, NULL, NULL, NULL, NULL },\n", m_name, dat->m_name);
				break;
			case DATUM_FLOAT:
				if(dat->m_maxSize < 0) {
					fprintf(outfile, "    { NULL, NULL, &%sRecord::Get%s, NULL, NULL, NULL, NULL },\n", m_name, dat->m_name);
				} else {
					fprintf(outfile, "    { &%sRecord::GetNum%s, NULL, NULL, NULL, NULL, NULL, &%sRecord::Get%s}, /* %s (array) */\n", m_name, dat->m_name, m_name, dat->m_name, dat->m_name);
				}
				break;
			case DATUM_BIT_PAIR:
				if(dat->m_bitPairDatum->m_type == DATUM_INT) {
					fprintf(outfile, "    { NULL, NULL, NULL, &%sRecord::Get%s, NULL, NULL, NULL },\n", m_name, dat->m_name);
				} else if(dat->m_bitPairDatum->m_type == DATUM_FLOAT) {
					fprintf(outfile, "    { NULL, NULL, NULL, NULL, &%sRecord::Get%s, NULL, NULL },\n", m_name, dat->m_name);
				} else {
					fprintf(outfile, "    { NULL, NULL, NULL, NULL, NULL, NULL, NULL }, /* %s */\n", dat->m_name);
				}
				
			default:
				fprintf(outfile, "    { NULL, NULL, NULL, NULL, NULL, NULL, NULL }, /* %s */\n", dat->m_name);
				break;
		}
		walk.Next();
	}
	fprintf(outfile, "};\n\n");
	
	walk.SetList(&m_datumList);
	sint32 numTokens = 0;
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		sprintf(nicename, "k_Token_%s_%s", m_name, dat->m_name);
		fprintf(outfile, "#define %-40s ((k_Token_Custom_Base) + %d)\n", nicename, numTokens);
		numTokens++;

		if(dat->m_type == DATUM_BIT_PAIR) {
			sprintf(nicename, "k_Token_%s_%s_Value", m_name, dat->m_name);
			fprintf(outfile, "#define %-40s ((k_Token_Custom_Base) + %d)\n", nicename, numTokens);
			numTokens++;
		}
		walk.Next();
	}

	walk.SetList(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		if(dat->m_akaName){
			sprintf(nicename, "k_Token_%s_%s", m_name, dat->m_akaName);
			fprintf(outfile, "#define %-40s ((k_Token_Custom_Base) + %d)\n", nicename, numTokens);
			numTokens++;
		}
		walk.Next();
	}

	sprintf(nicename, "k_Token_%s_Max", m_name);
	fprintf(outfile, "#define %-40s ((k_Token_Custom_Base) + %d)\n\n\n", nicename, numTokens);

	fprintf(outfile, "static BitArray s_ParsedTokens(%d);\n", numTokens);

	fprintf(outfile, "void %sRecord::CheckRequiredFields(DBLexer *lex)\n", m_name);
	fprintf(outfile, "{\n");
	if(g_generateRequirementWarnings) {
		fprintf(outfile, "    // Ick.\n");
		walk.SetList(&m_datumList);
		while(walk.IsValid()) {
			Datum *dat = walk.GetObj();
			if(dat->m_required) {
				if(dat->m_akaName){
					fprintf(outfile, "    if(!s_ParsedTokens.Bit(k_Token_%s_%s - k_Token_Custom_Base)\n", m_name, dat->m_name);
					fprintf(outfile, "    && !s_ParsedTokens.Bit(k_Token_%s_%s - k_Token_Custom_Base)\n", m_name, dat->m_akaName);
					fprintf(outfile, "    ){\n");
				}
				else{
					fprintf(outfile, "    if(!s_ParsedTokens.Bit(k_Token_%s_%s - k_Token_Custom_Base)) {\n", m_name, dat->m_name);
				}
				if(dat->m_defaultName){
					fprintf(outfile, "        m_%s = m_%s;\n", dat->m_name, dat->m_defaultName);
				}
				else{
					fprintf(outfile, "        DBERROR((\"Warning: required field %s missing\"));\n", dat->m_name);
				}
				fprintf(outfile, "    }\n");
			}
			walk.Next();
		}
	}
	fprintf(outfile, "}\n");

	fprintf(outfile, "\n");
	fprintf(outfile, "sint32 %sRecord::Parse(DBLexer *lex, sint32 numRecords)\n", m_name);
	fprintf(outfile, "{\n");
	if((m_baseType != DATUM_NONE)) {
		
		
		Assert(m_datumList.GetCount() == 2);
		fprintf(outfile, "    sint32 tok;\n");
		fprintf(outfile, "    tok = lex->GetToken();\n");
		fprintf(outfile, "    if(tok == k_Token_Int) {\n");
		fprintf(outfile, "        tok = lex->GetToken(); // Accept number to make new db compatible with the old database format\n");
		fprintf(outfile, "    }\n");
		fprintf(outfile, "    if(tok != k_Token_Name) {\n");
		fprintf(outfile, "        DBERROR((\"Record must start with name\"));\n");
		fprintf(outfile, "        return 0;\n");
		fprintf(outfile, "    }\n");
		fprintf(outfile, "    m_NameText = new char[strlen(lex->GetTokenText()) + 1];\n");
		fprintf(outfile, "    strcpy(m_NameText, lex->GetTokenText());\n");
		switch(m_baseType) {
			case DATUM_INT:
				fprintf(outfile, "    if(!lex->GetIntAssignment(m_Value)) {\n");
				fprintf(outfile, "        DBERROR((\"Expected integer\"));\n");
				fprintf(outfile, "        return 0;\n");
				fprintf(outfile, "    }\n");
				break;
			case DATUM_FLOAT:
				fprintf(outfile, "    if(!lex->GetFloatAssignment(m_Value)) {\n");
				fprintf(outfile, "        DBERROR((\"Expected number\"));\n");
				fprintf(outfile, "        return 0;\n");
				fprintf(outfile, "    }\n");
				break;
			case DATUM_STRING:
			case DATUM_FILE:
				fprintf(outfile, "    if(!lex->GetFileAssignment(m_Value)) {\n");
				fprintf(outfile, "        DBERROR((\"Expected string\"));\n");
				fprintf(outfile, "        return 0;\n");
				fprintf(outfile, "    }\n");
				break;
			case DATUM_STRINGID:
				fprintf(outfile, "    if(!lex->GetStringIdAssignment(m_Value)) {\n");
				fprintf(outfile, "        DBERROR((\"Expected string\"));\n");
				fprintf(outfile, "        return 0;\n");
				fprintf(outfile, "    }\n");
				break;
			default:
				Assert(0);
				break;
		}
		fprintf(outfile, "    return 1;\n");
		fprintf(outfile, "}\n");
	} else {
		
		char uppName[256];
		strcpy(uppName, m_name);
		for(short unsigned int i = 0; i < strlen(m_name); i++){
			uppName[i] = toupper(m_name[i]);
		}

		fprintf(outfile, "    bool done = false;\n");
		fprintf(outfile, "    sint32 result = 0;\n");
		fprintf(outfile, "    sint32 tok;\n");
		fprintf(outfile, "    DBPARSE_ERROR err = DBPARSE_OK;\n");
		fprintf(outfile, "    s_ParsedTokens.Clear();\n");
		fprintf(outfile, "    lex->SetTokens(g_%s_Tokens, k_Token_%s_Max);\n", m_name, m_name);
		fprintf(outfile, "    tok = lex->GetToken();\n");

		fprintf(outfile, "    if(tok == k_Token_Int) {\n");
		fprintf(outfile, "        tok = lex->GetToken(); // Accept number to make new db compatible with the old database format\n");
		fprintf(outfile, "    }\n");

		fprintf(outfile, "    if(tok != k_Token_Name) {\n");
		fprintf(outfile, "        char newName[256];\n");
		fprintf(outfile, "        sprintf(newName, \"%s_%s\", numRecords);\n", uppName, "%i");
		fprintf(outfile, "        if(!g_theStringDB->GetStringID(newName, m_name)) {\n");
		
		
		fprintf(outfile, "            g_theStringDB->InsertStr(newName, newName);\n");
		fprintf(outfile, "            if(!g_theStringDB->GetStringID(newName, m_name))\n");
		fprintf(outfile, "                SetTextName(newName);\n");
		fprintf(outfile, "        }\n");
		fprintf(outfile, "    }\n");
		fprintf(outfile, "    else{\n");
		fprintf(outfile, "        if(!g_theStringDB->GetStringID(lex->GetTokenText(), m_name)) {\n");
		
		
		fprintf(outfile, "            g_theStringDB->InsertStr(lex->GetTokenText(), lex->GetTokenText());\n");
		fprintf(outfile, "            if(!g_theStringDB->GetStringID(lex->GetTokenText(), m_name))\n");
		fprintf(outfile, "                SetTextName(lex->GetTokenText());\n");
		fprintf(outfile, "        }\n");
		fprintf(outfile, "        tok = lex->GetToken();\n");
		fprintf(outfile, "    }\n");
		fprintf(outfile, "\n");

		if(m_hasGovernmentsModified) {
		fprintf(outfile, "    // Start of GovMod Specific lexical analysis\n");
		fprintf(outfile, "    if(tok == k_Token_Modified) {\n");
		fprintf(outfile, "        do {\n");
		fprintf(outfile, "            tok = lex->PeekAhead();\n");
		fprintf(outfile, "            if(tok != k_Token_Name) {\n");
		fprintf(outfile, "                DBERROR((\"Modified record invalid- must be Government identifier or description.  No quotes, No spaces.\"));\n");
		fprintf(outfile, "                return 0;\n");
		fprintf(outfile, "            }\n");
		fprintf(outfile, "            g_theGovernmentDB->ParseRecordInArray(lex, (sint32 **)&m_GovernmentsModified, &m_numGovernmentsModified, err);\n");
		fprintf(outfile, "            tok = lex->GetToken();\n");
		fprintf(outfile, "        } while (tok == k_Token_ModifiedDelimiter);\n");
		fprintf(outfile, "    }\n");
		fprintf(outfile, "    // End of GovMod Specific lexical analysis\n");
		}

		fprintf(outfile, "    if(tok != k_Token_OpenBrace) {\n");
		fprintf(outfile, "        DBERROR((\"Missing open brace\"));\n");
		fprintf(outfile, "        return 0;\n");
		fprintf(outfile, "    }\n");
		fprintf(outfile, "\n");
		fprintf(outfile, "    while(!done) {\n");
		fprintf(outfile, "        tok = lex->GetToken();\n");
		fprintf(outfile, "        if(tok >= k_Token_Custom_Base && tok < k_Token_%s_Max) {\n", m_name);
		fprintf(outfile, "            s_ParsedTokens.SetBit(tok - k_Token_Custom_Base);\n");
		fprintf(outfile, "        }\n");
		fprintf(outfile, "        switch(tok) {\n");
		
		
		ExportTokenCases(outfile);
		
		
		fprintf(outfile, "            case k_Token_CloseBrace:\n");
		fprintf(outfile, "                done = true;\n");
		fprintf(outfile, "                result = 1;\n");
		fprintf(outfile, "                break;\n");
		fprintf(outfile, "            default:\n");
		fprintf(outfile, "                DBERROR((\"Unknown token\"));\n");
		fprintf(outfile, "                done = true;\n");
		fprintf(outfile, "                break;\n");
		fprintf(outfile, "        }\n");
		fprintf(outfile, "    }\n");
		fprintf(outfile, "    CheckRequiredFields(lex);\n");
		fprintf(outfile, "    lex->RestoreTokens();\n");
		fprintf(outfile, "    return result;\n");
		fprintf(outfile, "}\n\n");
	}
	
	ExportResolver(outfile);
	
	ExportMemberClassParsers(outfile);
	
	ExportDataParsers(outfile);
}
		
void RecordDescription::ExportTokenCases(FILE *outfile)
{
	
	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		if(dat->m_type == DATUM_BIT_PAIR) {
			dat->ExportParseBitPairCase(outfile, m_name);
		}
		fprintf(outfile,         "            case k_Token_%s_%s:\n", m_name, dat->m_name);
		if(dat->m_akaName){
			fprintf(outfile,         "            case k_Token_%s_%s:\n", m_name, dat->m_akaName);
		}
		if(dat->m_maxSize == k_MAX_SIZE_VARIABLE) {
			
			switch(dat->m_type) {
				case DATUM_INT:
					fprintf(outfile, "                if(!CTPRecord::ParseIntInArray(lex, &m_%s, &m_num%s))\n", dat->m_name, dat->m_name);
					break;
				case DATUM_STRINGID:
					fprintf(outfile, "                if(!CTPRecord::ParseStringIdInArray(lex, &m_%s, &m_num%s))\n", dat->m_name, dat->m_name);
					break;
				case DATUM_FLOAT:
					fprintf(outfile, "                if(!CTPRecord::ParseFloatInArray(lex, &m_%s, &m_num%s))\n", dat->m_name, dat->m_name);
					break;
				case DATUM_STRING:
				case DATUM_FILE:
					fprintf(outfile, "                if(!CTPRecord::ParseFileInArray(lex, &m_%s, &m_num%s))\n", dat->m_name, dat->m_name);
					break;
				case DATUM_RECORD:
					fprintf(outfile, "                if(!g_the%sDB->ParseRecordInArray(lex, (sint32 **)&m_%s, &m_num%s, err))\n", dat->m_subType, dat->m_name, dat->m_name);

					break;
				case DATUM_STRUCT:
					fprintf(outfile, "                if(!%sRecord::%s::ParseInArray(lex, &m_%s, &m_num%s))\n", m_name, dat->m_subType, dat->m_name, dat->m_name);
					break;
				default:
					Assert(0);
					break;
			}
			fprintf(outfile, "                {\n");
			fprintf(outfile, "                    done = true; break;\n");
			fprintf(outfile, "                }\n");
		} else if(dat->m_maxSize > 0) {
			
			switch(dat->m_type) {
				case DATUM_INT:
					fprintf(outfile, "                if(!CTPRecord::ParseIntInArray(lex, (sint32 *)m_%s, &m_num%s, k_MAX_%s)) {\n", 
							dat->m_name, dat->m_name, dat->m_name);
					break;
				case DATUM_STRINGID:
					fprintf(outfile, "                if(!CTPRecord::ParseStringIdInArray(lex, (sint32 *)m_%s, &m_num%s, k_MAX_%s)) {\n",
							dat->m_name, dat->m_name, dat->m_name);
					break;
				case DATUM_FLOAT:
					fprintf(outfile, "                if(!CTPRecord::ParseFloatInArray(lex, (double *)m_%s, &m_num%s, k_MAX_%s)) {\n", 
							dat->m_name, dat->m_name, dat->m_name);
					break;
				case DATUM_STRING:
				case DATUM_FILE:
					fprintf(outfile, "                if(!CTPRecord::ParseFileInArray(lex, (char **)m_%s, &m_num%s, k_MAX_%s)) {\n", 
							dat->m_name, dat->m_name, dat->m_name);
					break;
				case DATUM_RECORD:
					fprintf(outfile, "                if(!g_the%sDB->ParseRecordInArray(lex, (sint32 *)m_%s, &m_num%s, k_MAX_%s, err)) {\n", 
							dat->m_subType, dat->m_name, dat->m_name, dat->m_name);
					break;
				case DATUM_STRUCT:
					fprintf(outfile, "                if(!%sRecord::%s::ParseInArray(lex, (sint32 *)m_%s, &m_num%s, k_MAX_%s)) {\n", 
							m_name, dat->m_subType, dat->m_name, dat->m_name, dat->m_name);
					break;
				default:
					Assert(0);
					break;
			}
			fprintf(outfile, "                    done = true; break;\n");
			fprintf(outfile, "                }\n");
		} else if(dat->m_maxSize < 0) {
			switch(dat->m_type) {
				case DATUM_INT:
					fprintf(outfile, "                if(!lex->GetIntAssignment(m_%s)) {\n", dat->m_name);
					fprintf(outfile, "                    DBERROR((\"Expected integer\"));\n");
					fprintf(outfile, "                    done = true; break;\n");
					fprintf(outfile, "                }\n");
					break;
				case DATUM_STRINGID:
					fprintf(outfile, "                if(!lex->GetStringIdAssignment(m_%s)) {\n", dat->m_name);
					fprintf(outfile, "                    DBERROR((\"Expected string ID\"));\n");
					fprintf(outfile, "                    done = true; break;\n");
					fprintf(outfile, "                }\n");
					break;
				case DATUM_BIT:
					fprintf(outfile, "                m_flags%d |= k_%s_%s_Bit;\n", dat->m_bitNum / 32,
							m_name, dat->m_name);
					break;
				case DATUM_BIT_PAIR:
					fprintf(outfile, "                m_flags%d |= k_%s_%s_Bit;\n", dat->m_bitNum / 32,
							m_name, dat->m_name);
					dat->ExportBitPairDirectParse(outfile, m_name);
					break;
				case DATUM_FLOAT:
					fprintf(outfile, "                if(!lex->GetFloatAssignment(m_%s)) {\n", dat->m_name);
					fprintf(outfile, "                    DBERROR((\"Expected number\"));\n");
					fprintf(outfile, "                    done = true; break;\n");
					fprintf(outfile, "                }\n");
					break;
				case DATUM_STRING:
				case DATUM_FILE:
					fprintf(outfile, "                if(!lex->GetFileAssignment(m_%s)) {\n", dat->m_name);
					fprintf(outfile, "                    DBERROR((\"Expected string\"));\n");
					fprintf(outfile, "                    done = true; break;\n");
					fprintf(outfile, "                }\n");
					break;
				case DATUM_RECORD:
					fprintf(outfile, "                if(!g_the%sDB->GetRecordFromLexer(lex, m_%s, err)) {\n", dat->m_subType, dat->m_name);
					fprintf(outfile, "                    done = true; break;\n");
					fprintf(outfile, "                }\n");
					break;
				case DATUM_STRUCT:
					fprintf(outfile, "                if(!m_%s.Parse(lex)) {\n", dat->m_name);
					fprintf(outfile, "                    done = true; break;\n");
					fprintf(outfile, "                }\n");
					break;
				case DATUM_BIT_GROUP:
					fprintf(outfile, "                if(!Parse%sBit(lex)) {\n", dat->m_name);
					fprintf(outfile, "                    done = true; break;\n");
					fprintf(outfile, "                }\n");
					break;
				default:
					Assert(0);
					break;
			}
		}
		fprintf(outfile,         "                break;\n");
		walk.Next();
	}
}

void RecordDescription::ExportMemberClassParsers(FILE *outfile)
{
	fprintf(outfile, "/*\n * Member class Parsers\n * (DO NOT EDIT!  Automatically generated file)\n */\n");
	
	PointerList<MemberClass>::Walker walk(&m_memberClasses);
	while(walk.IsValid()) {
		walk.GetObj()->ExportInitialization(outfile, m_name);
		walk.GetObj()->ExportParser(outfile, m_name);
		walk.Next();
	}
}
	
void RecordDescription::ExportDataParsers(FILE *outfile)
{
	fprintf(outfile, "/*\n * Data Parsers\n * (DO NOT EDIT!  Automatically generated file)\n */\n");

	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		switch(dat->m_type) {
			default:
				break;
			case DATUM_BIT_GROUP:
				dat->ExportBitGroupParser(outfile, m_name);
				break;
		}
		walk.Next();
	}
}

#if 0
void RecordDescription::ExportDBHeader(FILE *outfile)
{
	fprintf(outfile, "#ifndef __%s_DB_H__\n", m_name);
	fprintf(outfile, "#define __%s_DB_H__\n", m_name);

	fprintf(outfile, "class %sDatabase : public CTPDatabase<%sRecord>\n", m_name, m_name);
	fprintf(outfile, "{\n");
	fprintf(outfile, "public:\n");
	fprintf(outfile, "\n");
	fprintf(outfile, "#endif /* __%s_DB_H__ */\n", m_name);
}
#endif

void RecordDescription::ExportMemberClassDataCode(FILE *outfile)
{
	PointerList<MemberClass>::Walker walk(&m_memberClasses);
	while(walk.IsValid()) {
		walk.GetObj()->ExportDataCode(outfile, m_name);
		walk.Next();
	}
}


void RecordDescription::ExportResolver(FILE *outfile)
{
	fprintf(outfile, "void %sRecord::ResolveDBReferences()\n", m_name);
	fprintf(outfile, "{\n");
	PointerList<Datum>::Walker walk(&m_datumList);
	while(walk.IsValid()) {
		Datum *dat = walk.GetObj();
		if(dat->m_type == DATUM_RECORD ||
		   dat->m_type == DATUM_STRUCT ||
		   dat->m_type == DATUM_BIT_PAIR) {
			dat->ExportResolver(outfile);
		}
		walk.Next();
	}
	fprintf(outfile, "}\n");
}

