//----------------------------------------------------------------------------
//
// Project : Call To Power 2
// File type : C++ header
// Description : Activision User Interface - ldl handling
// Id : $Id$
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
// - Fixed memory leaks.
//
//----------------------------------------------------------------------------

#ifndef __AUI_LDL_H__
#define __AUI_LDL_H__

#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include "ui/aui_common/aui_base.h"
#include "ui/aui_common/aui_control.h"

class aui_Region;
class aui_Dimension;
class ldl;
class ldl_datablock;
template <class T> class AvlTree;

struct aui_LdlObject {
  void *object;
  std::string ldlBlock;
  struct ByObject {};
  struct ByLdlBlock {};
};

typedef std::list<aui_LdlObject> LDLObjectList;

#define k_AUI_LDL_MAXBLOCK 1024

#define k_AUI_LDL_DEFAULTTEMPLATE "default"

#define k_AUI_LDL_PARENT "parent"
#define k_AUI_LDL_HANCHOR "xanchor"
#define k_AUI_LDL_VANCHOR "yanchor"
#define k_AUI_LDL_HABSPOSITION "xpix"
#define k_AUI_LDL_HRELPOSITION "xpct"
#define k_AUI_LDL_VABSPOSITION "ypix"
#define k_AUI_LDL_VRELPOSITION "ypct"
#define k_AUI_LDL_HABSSIZE "widthpix"
#define k_AUI_LDL_HRELSIZE "widthpct"
#define k_AUI_LDL_VABSSIZE "heightpix"
#define k_AUI_LDL_VRELSIZE "heightpct"

#define k_AUI_LDL_OBJECTTYPE "objecttype"
#define k_AUI_LDL_ATOMIC "atomic"
#define k_AUI_LDL_DETACH "detach"

typedef boost::multi_index_container<
  aui_LdlObject,
  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<
      boost::multi_index::tag<aui_LdlObject::ByObject>,
      boost::multi_index::member<aui_LdlObject, void*, &aui_LdlObject::object>
    >,
    boost::multi_index::ordered_unique<
      boost::multi_index::tag<aui_LdlObject::ByLdlBlock>, 
      boost::multi_index::member<aui_LdlObject, std::string, &aui_LdlObject::ldlBlock>
    >
  >
> LdlObjectRegistry;

class aui_Ldl : public aui_Base {
public:
  aui_Ldl
  (
    AUI_ERRCODE * retval,
    MBCHAR const * ldlFilename
  );
  virtual ~aui_Ldl();

public:
  static bool IsValid(MBCHAR const *ldlBlock);
  static ldl *GetLdl(void) { return s_ldl; }

  static AUI_ERRCODE Associate(void *object, const MBCHAR *ldlBlock);
  static AUI_ERRCODE Remove(void *object);
  static AUI_ERRCODE Remove(MBCHAR const * ldlBlock);
  static const MBCHAR *GetBlock(void *object);
  static void *GetObject(const std::string &ldlBlock);
  static void *GetObject(const MBCHAR *ldlBlock);
  static void *GetObject(const MBCHAR *parentBlock, const MBCHAR *regionBlock);

  static AUI_ERRCODE SetupHeirarchyFromRoot(const MBCHAR *rootBlock);
  static AUI_ERRCODE SetupHeirarchyFromLeaf(const MBCHAR *leafBlock, aui_Region *object);
  static aui_Region * BuildHierarchyFromRoot(const MBCHAR *rootBlock);
  static AUI_ERRCODE BuildHierarchyFromLeaf(ldl_datablock *dataBlock, aui_Region *region);
  static AUI_ERRCODE BuildObjectFromType(const MBCHAR *typeString, const MBCHAR *ldlName,
    aui_Region **theObject);
  static AUI_ERRCODE DeleteHierarchyFromRoot(MBCHAR const * rootBlock);
  static AUI_ERRCODE DeleteHierarchyFromLeaf(ldl_datablock *dataBlock);

  static AUI_ERRCODE SetActionFuncAndCookie
  (
    MBCHAR const * ldlBlock,
    aui_Control::ControlActionCallback * actionFunc,
    void * cookie = NULL
  );

  static AUI_ERRCODE SetActionFuncAndCookie
  (
    MBCHAR const * parentBlock,
    MBCHAR const * regionBlock,
    aui_Control::ControlActionCallback * actionFunc,
    void * cookie = NULL
  );

  static AUI_ERRCODE DetachHierarchy(aui_Region *root);

  static void ModifyAttributes(MBCHAR const *ldlBlock, aui_Dimension * dimension);

  static sint32 GetIntDependent(MBCHAR const * strPtr);
  static sint32 GetIntDependent(const std::string &str);
  static ldl_datablock * FindDataBlock(MBCHAR const * ldlBlock);

protected:
  static AUI_ERRCODE MakeSureBlockExists(MBCHAR const * ldlBlock);
  static AUI_ERRCODE MakeSureDefaultTemplateExists(void);

  static ldl *s_ldl;

  static LdlObjectRegistry s_ldlObjectRegistry;

  static sint32 s_ldlRefCount;

private:
  AUI_ERRCODE InitCommon(MBCHAR const * ldlFilename);

};

#endif
