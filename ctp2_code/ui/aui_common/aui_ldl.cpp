//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Activision User Interface - ldl handling
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
// - Fixed memory leaks.
//
//----------------------------------------------------------------------------

#include "ctp/c3.h"

#include <boost/scoped_array.hpp>

#include "ui/aui_common/aui_ldl.h"

#include "ui/aui_common/aui_ui.h"
#include "ui/aui_common/aui_dimension.h"
#include "ui/aui_common/aui_uniqueid.h"

#include "robot/aibackdoor/avl.h"

#include "ui/aui_ctp2/c3slider.h"
#include "ui/aui_ctp2/c3window.h"
#include "ui/aui_ctp2/c3windows.h"
#include "ui/aui_ctp2/c3_button.h"
#include "ui/aui_ctp2/c3_static.h"
#include "ui/aui_ctp2/ctp2_Static.h"
#include "ui/aui_ctp2/ctp2_button.h"
#include "ui/aui_ctp2/ctp2_Window.h"
#include "ui/aui_ctp2/ctp2_menubar.h"
#include "ui/aui_ctp2/ctp2_listbox.h"
#include "ui/aui_ctp2/ctp2_listitem.h"
#include "ui/aui_ctp2/ctp2_dropdown.h"
#include "ui/aui_ctp2/ctp2_spinner.h"
#include "ui/aui_ctp2/ctp2_hypertextbox.h"
#include "ui/aui_ctp2/c3_hypertextbox.h"
#include "ui/aui_common/aui_switch.h"
#include "gfx/gfx_utils/colorset.h"
#include "ui/aui_ctp2/ctp2_Switch.h"
#include "ui/aui_ctp2/ctp2_textfield.h"
#include "ui/aui_ctp2/ctp2_MenuButton.h"
#include "ui/aui_ctp2/ctp2_TabGroup.h"
#include "ui/aui_ctp2/ctp2_Tab.h"
#include "ui/aui_ctp2/ctp2_TabButton.h"
#include "ui/aui_ctp2/ctp2_TabGroup.h"
#include "ui/aui_ctp2/radarmap.h"
#include "ui/aui_ctp2/linegraph.h"
#include "ctp/ctp2_utils/AvlTree.h"

#include "ui/ldl/ldl_user.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

ldl              *aui_Ldl::s_ldl = NULL;
sint32						aui_Ldl::s_ldlRefCount = 0;
LdlObjectRegistry aui_Ldl::s_ldlObjectRegistry;


aui_Ldl::aui_Ldl
(
  AUI_ERRCODE *   retval,
  MBCHAR const *  ldlFilename
)
  :
  aui_Base() {
  *retval = InitCommon(ldlFilename);
}


AUI_ERRCODE aui_Ldl::InitCommon(MBCHAR const *ldlFilename) {
  Assert(ldlFilename != NULL);
  if (!ldlFilename) return AUI_ERRCODE_INVALIDPARAM;

  MBCHAR outDir[MAX_PATH + 1];
#ifdef HAVE_UNISTD_H
  getcwd(outDir, MAX_PATH);
#else
  GetCurrentDirectory(MAX_PATH, outDir);
#endif
  strcat(outDir, FILE_SEP "ldl_out");

  delete s_ldl;
  s_ldl = new ldl(ldlFilename, outDir);
  Assert(s_ldl != NULL);
  if (!s_ldl) return AUI_ERRCODE_MEMALLOCFAILED;

  s_ldlRefCount++;

  return AUI_ERRCODE_OK;
}


aui_Ldl::~aui_Ldl() {
  if (0 == --s_ldlRefCount) {
    delete s_ldl;
    s_ldl = NULL;
    s_ldlObjectRegistry.clear();
  }
}


bool aui_Ldl::IsValid(MBCHAR const * ldlBlock) {
  return FindDataBlock(ldlBlock) != NULL;
}


AUI_ERRCODE aui_Ldl::MakeSureBlockExists(MBCHAR const *ldlBlock) {
  Assert(ldlBlock != NULL);
  if (!ldlBlock) return AUI_ERRCODE_INVALIDPARAM;

  ldl_datablock *block = s_ldl->FindDataBlock(ldlBlock);
  if (!block) {
#if 1
    LDLDataBlockPtr block = std::make_shared<ldl_datablock>(s_ldl, ldlBlock);

    block->AddAttribute(k_AUI_LDL_HANCHOR, "left");
    block->AddAttribute(k_AUI_LDL_VANCHOR, "top");
    block->AddAttribute(k_AUI_LDL_HABSPOSITION, 0);
    block->AddAttribute(k_AUI_LDL_VABSPOSITION, 0);
    block->AddAttribute(k_AUI_LDL_HABSSIZE, 100);
    block->AddAttribute(k_AUI_LDL_VABSSIZE, 100);

    BOOL success = s_ldl->WriteData();
    Assert(success);
    if (!success) return AUI_ERRCODE_LDLFILEWRITEFAILED;
#else
    return AUI_ERRCODE_LDLFINDDATABLOCKFAILED;
#endif
  }

  return AUI_ERRCODE_OK;
}


AUI_ERRCODE aui_Ldl::MakeSureDefaultTemplateExists(void) {
  ldl_datablock *format = s_ldl->FindDataBlock(k_AUI_LDL_DEFAULTTEMPLATE);
  if (!format) {

#if 1

    format = new ldl_datablock(s_ldl, k_AUI_LDL_DEFAULTTEMPLATE);
    Assert(format != NULL);
    if (!format) return AUI_ERRCODE_MEMALLOCFAILED;

    format->AddAttribute(k_AUI_LDL_HANCHOR, "left");

    format->AddAttribute(k_AUI_LDL_VANCHOR, "top");

    format->AddAttribute(k_AUI_LDL_HABSPOSITION, 0);
    format->AddAttribute(k_AUI_LDL_VABSPOSITION, 0);
    format->AddAttribute(k_AUI_LDL_HABSSIZE, 100);
    format->AddAttribute(k_AUI_LDL_VABSSIZE, 100);

    BOOL success = s_ldl->WriteData();
    Assert(success);
    if (!success) return AUI_ERRCODE_LDLFILEWRITEFAILED;
#else
    return AUI_ERRCODE_LDLFINDDATABLOCKFAILED;
#endif
  }

  return AUI_ERRCODE_OK;
}


AUI_ERRCODE aui_Ldl::Associate(void *object, MBCHAR const * ldlBlock) {
  Assert(object != NULL);
  if (!object) return AUI_ERRCODE_INVALIDPARAM;
  Assert(ldlBlock != NULL);
  if (!ldlBlock) return AUI_ERRCODE_INVALIDPARAM;

  if (GetBlock(object)) return AUI_ERRCODE_OK;

  aui_LdlObject ldlObject;

  ldlObject.object = object;
  ldlObject.ldlBlock = ldlBlock;

  return s_ldlObjectRegistry.insert(ldlObject).second ? AUI_ERRCODE_OK : AUI_ERRCODE_INVALIDPARAM;
}

AUI_ERRCODE aui_Ldl::Remove(void *object) {
  if (!object)
    return AUI_ERRCODE_INVALIDPARAM;

  auto &byObj = s_ldlObjectRegistry.get<aui_LdlObject::ByObject>();
  auto itObj = byObj.find(object);
  if (itObj != byObj.end()) {
    byObj.erase(itObj);
  }

  return AUI_ERRCODE_OK;
}


AUI_ERRCODE aui_Ldl::Remove(MBCHAR const * ldlBlock) {
  auto &byLdlBlock = s_ldlObjectRegistry.get<aui_LdlObject::ByLdlBlock>();
  auto itObj = byLdlBlock.find(std::string(ldlBlock));
  if (itObj != byLdlBlock.end()) {
    byLdlBlock.erase(itObj);
  }

  return AUI_ERRCODE_OK;
}


const MBCHAR *aui_Ldl::GetBlock(void *object) {
  auto const &byObj = s_ldlObjectRegistry.get<aui_LdlObject::ByObject>();
  auto itObj = byObj.find(object);
  if (itObj != byObj.end()) {
    return itObj->ldlBlock.c_str();
  }

  return NULL;
}

void *aui_Ldl::GetObject(const std::string &ldlBlock) {
  return aui_Ldl::GetObject(ldlBlock.c_str());
}

void *aui_Ldl::GetObject(const MBCHAR *ldlBlock) {
  auto const &byLdlBlock = s_ldlObjectRegistry.get<aui_LdlObject::ByLdlBlock>();
  auto itObj = byLdlBlock.find(std::string(ldlBlock));
  if (itObj != byLdlBlock.end()) {
    return itObj->object;
  }

  return NULL;
}

void *aui_Ldl::GetObject(const MBCHAR *parentBlock, const MBCHAR *regionBlock) {
  Assert(parentBlock && regionBlock);
  if (parentBlock == NULL || regionBlock == NULL)
    return NULL;

  const std::string ldlBlock = std::string(parentBlock) + "." + regionBlock;

  return GetObject(ldlBlock.c_str());
}

AUI_ERRCODE aui_Ldl::SetupHeirarchyFromRoot(MBCHAR const * rootBlock) {
  Assert(rootBlock != NULL);
  if (!rootBlock) return AUI_ERRCODE_INVALIDPARAM;

  auto &byLdlBlock = s_ldlObjectRegistry.get<aui_LdlObject::ByLdlBlock>();
  auto itObj = byLdlBlock.find(std::string(rootBlock));
  if (itObj != byLdlBlock.end()) {
    const aui_LdlObject &obj = *itObj;
    AUI_ERRCODE errcode = SetupHeirarchyFromLeaf(obj.ldlBlock.c_str(), (aui_Region *)obj.object);
    Assert(AUI_SUCCESS(errcode));
    if (!AUI_SUCCESS(errcode)) return errcode;
  }

  Assert(FALSE);
  return AUI_ERRCODE_HACK;
}


AUI_ERRCODE aui_Ldl::SetupHeirarchyFromLeaf(const MBCHAR *leafBlock, aui_Region *object) {
  Assert(leafBlock != NULL && object != NULL);
  if (!leafBlock || !object)
    return AUI_ERRCODE_INVALIDPARAM;

  std::string strLeafBlock(leafBlock);
  const std::string::size_type lastDotPos = strLeafBlock.find_last_of('.');
  if (lastDotPos == std::string::npos) {
    return AUI_ERRCODE_OK;
  }

  if (object->GetParent())
    return AUI_ERRCODE_OK;

  const std::string parentBlock = strLeafBlock.substr(0, lastDotPos);

  aui_Region *parent = (aui_Region *)GetObject(parentBlock.c_str());
  Assert(parent != NULL);
  if (!parent)
    return AUI_ERRCODE_HACK;

  parent->AddChild(object);

  AUI_ERRCODE errcode = SetupHeirarchyFromLeaf(parentBlock.c_str(), parent);
  Assert(AUI_SUCCESS(errcode));
  if (!AUI_SUCCESS(errcode))
    return errcode;

  return AUI_ERRCODE_OK;
}

aui_Region *aui_Ldl::BuildHierarchyFromRoot(MBCHAR const * rootBlock) {
  Assert(rootBlock);
  if (rootBlock == NULL)
    return NULL;

  ldl_datablock * dataBlock = s_ldl->FindDataBlock(rootBlock, NULL);
  Assert(dataBlock);
  if (!dataBlock)
    return NULL;

  const std::string objTypeString = dataBlock->GetString(k_AUI_LDL_OBJECTTYPE);
  if (objTypeString.empty())
    return NULL;

  bool			isAtomic = dataBlock->GetBool(k_AUI_LDL_ATOMIC);
  std::string fullname = dataBlock->GetFullName();

  aui_Region *    myRegion = NULL;
  AUI_ERRCODE		err = BuildObjectFromType(objTypeString.c_str(), fullname.c_str(), &myRegion);
  Assert(err == AUI_ERRCODE_OK);
  if (err != AUI_ERRCODE_OK)
    return NULL;

  if (!isAtomic) {
    err = BuildHierarchyFromLeaf(dataBlock, myRegion);
    Assert(err == AUI_ERRCODE_OK);
    if (err != AUI_ERRCODE_OK)
      return NULL;
  }

  err = SetupHeirarchyFromRoot(rootBlock);
  Assert(err == AUI_ERRCODE_OK);
  if (err != AUI_ERRCODE_OK)
    return NULL;

  if (dataBlock->GetBool(k_AUI_LDL_DETACH)) {
    DetachHierarchy(myRegion);
  }

  myRegion->DoneInstantiating();

  return myRegion;
}


AUI_ERRCODE aui_Ldl::BuildHierarchyFromLeaf(ldl_datablock *parent, aui_Region *region) {
  for (LDLDataBlockPtr dataBlock : parent->GetChildList()) {
    Assert(dataBlock);
    if (!dataBlock)
      return AUI_ERRCODE_INVALIDPARAM;

    const std::string objTypeString = dataBlock->GetString(k_AUI_LDL_OBJECTTYPE);
    if (objTypeString.empty())
      return AUI_ERRCODE_INVALIDPARAM;

    BOOL			isAtomic = dataBlock->GetBool(k_AUI_LDL_ATOMIC);
    const std::string fullname = dataBlock->GetFullName();

    aui_Region *    myRegion;
    AUI_ERRCODE		err = BuildObjectFromType(objTypeString.c_str(), fullname.c_str(), &myRegion);
    Assert(err == AUI_ERRCODE_OK);
    if (err != AUI_ERRCODE_OK)
      return AUI_ERRCODE_INVALIDPARAM;

    if (!isAtomic) {
      err = BuildHierarchyFromLeaf(dataBlock.get(), myRegion); // TODO: replace dataBlock.get()
      Assert(err == AUI_ERRCODE_OK);
      if (err != AUI_ERRCODE_OK)
        return err;
    }

    region->AddChild(myRegion);
  }

  return AUI_ERRCODE_OK;
}

AUI_ERRCODE aui_Ldl::BuildObjectFromType(const MBCHAR *typeString,
  const MBCHAR *ldlBlockName,
  aui_Region **theObject) {
  MBCHAR *ldlName = const_cast<MBCHAR *>(ldlBlockName); // For compatibility TODO: remove

  AUI_ERRCODE		retval = AUI_ERRCODE_OK;
  aui_Region		*region = NULL;

  if (!stricmp(typeString, "C3Window")) {
    region = (aui_Region *) new C3Window(&retval, aui_UniqueId(), ldlName, 16);
  } else if (!stricmp(typeString, "ctp2_Window")) {
    region = (aui_Region *) new ctp2_Window(&retval, aui_UniqueId(), ldlName, 16);
  } else if (!stricmp(typeString, "C3Slider")) {
    region = (aui_Region *) new C3Slider(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "c3_Button")) {
    region = (aui_Region *) new c3_Button(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "c3_Static")) {
    region = (aui_Region *) new c3_Static(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "ctp2_Static")) {
    region = (aui_Region *) new ctp2_Static(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "ctp2_Button")) {
    region = (aui_Region *) new ctp2_Button(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "ctp2_ListBox")) {
    region = (aui_Region *) new ctp2_ListBox(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "ctp2_LineGraph")) {
    region = (aui_Region *) new LineGraph(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "ctp2_ListItem")) {
    region = (aui_Region *) new ctp2_ListItem(&retval, ldlName);
  } else if (!stricmp(typeString, "ctp2_DropDown")) {
    region = (aui_Region *) new ctp2_DropDown(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "ctp2_Spinner")) {
    region = (aui_Region *) new ctp2_Spinner(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "ctp2_HyperTextBox")) {
    region = (aui_Region *)new ctp2_HyperTextBox(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "c3_HyperTextBox")) {
    region = (aui_Region *)new c3_HyperTextBox(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "ctp2_MenuBar")) {
    region = (aui_Region *) new ctp2_MenuBar(&retval, aui_UniqueId(), ldlName, 16);
  } else if (!stricmp(typeString, "aui_switch")) {
    region = (aui_Region *) new aui_Switch(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "ctp2_Switch")) {
    region = (aui_Region *) new ctp2_Switch(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "ctp2_TextField")) {
    region = (aui_Region *) new ctp2_TextField(&retval, aui_UniqueId(), ldlName);
  } else if (!stricmp(typeString, "ctp2_MenuButton")) {
    region = (aui_Region *) new ctp2_MenuButton(&retval, aui_UniqueId(), ldlName);
  }
  if (!stricmp(typeString, "ctp2_Tab")) {
    region = (aui_Region *) new ctp2_Tab(&retval, aui_UniqueId(), ldlName);
  }
  if (!stricmp(typeString, "ctp2_TabGroup")) {
    region = (aui_Region *) new ctp2_TabGroup(&retval, aui_UniqueId(), ldlName);
  }
  if (!stricmp(typeString, "ctp2_TabButton")) {
    region = (aui_Region *) new ctp2_TabButton(&retval, aui_UniqueId(), ldlName);
  }
  if (!stricmp(typeString, "RadarMap")) {
    region = (aui_Region *) new RadarMap(&retval, aui_UniqueId(), ldlName);
  }

  Assert(region);
  Assert(retval == AUI_ERRCODE_OK);
  if (!region || retval != AUI_ERRCODE_OK) {

    c3errors_ErrorDialog("aui_Ldl::BuildObjectFromType()",
      "Auto-instantiate of '%s' failed on '%s'",
      typeString,
      ldlName);

    *theObject = NULL;

    return AUI_ERRCODE_INVALIDPARAM;
  }

  *theObject = region;

  return AUI_ERRCODE_OK;
}






AUI_ERRCODE aui_Ldl::DeleteHierarchyFromRoot(MBCHAR const * rootBlock) {
  Assert(rootBlock);
  if (rootBlock == NULL) return AUI_ERRCODE_INVALIDPARAM;


  ldl_datablock * dataBlock = s_ldl->FindDataBlock(rootBlock, NULL);
  Assert(dataBlock);
  if (!dataBlock) return AUI_ERRCODE_INVALIDPARAM;


  AUI_ERRCODE		errcode = DeleteHierarchyFromLeaf(dataBlock);
  Assert(errcode == AUI_ERRCODE_OK);
  if (errcode != AUI_ERRCODE_OK)
    return errcode;

  const std::string fullname = dataBlock->GetFullName();
  aui_Region *region = (aui_Region *)GetObject(fullname.c_str());
  Assert(region);
  if (region) {
    delete region;
  } else {
    return AUI_ERRCODE_INVALIDPARAM;
  }

  return AUI_ERRCODE_OK;
}


AUI_ERRCODE aui_Ldl::DeleteHierarchyFromLeaf(ldl_datablock *parent) {
  AUI_ERRCODE		errcode = AUI_ERRCODE_OK;
  aui_Region		*region;
  
  if (parent == NULL) return AUI_ERRCODE_OK;

  for (LDLDataBlockPtr dataBlock : parent->GetChildList()) {
    BOOL isAtomic = dataBlock->GetBool(k_AUI_LDL_ATOMIC);

    if (!isAtomic) {
      errcode = DeleteHierarchyFromLeaf(dataBlock.get()); // For compatibility. TODO: replace
      Assert(errcode == AUI_ERRCODE_OK);
      if (errcode != AUI_ERRCODE_OK)
        return errcode;
    }

    region = (aui_Region *)GetObject(dataBlock->GetFullName().c_str());
    Assert(region);
    if (!region)
      return AUI_ERRCODE_INVALIDPARAM;

    delete region;
  }

  return errcode;
}


AUI_ERRCODE aui_Ldl::SetActionFuncAndCookie
(
  MBCHAR const *                          ldlBlock,
  aui_Control::ControlActionCallback *    actionFunc,
  void *                                  cookie
) {
  aui_Control	*   control = (aui_Control *)aui_Ldl::GetObject(ldlBlock);
  AUI_ERRCODE		errcode = control
    ? control->SetActionFuncAndCookie(actionFunc, cookie)
    : AUI_ERRCODE_NOCONTROL;
  Assert(errcode == AUI_ERRCODE_OK);

  return errcode;
}

AUI_ERRCODE aui_Ldl::SetActionFuncAndCookie
(
  MBCHAR const *                          parentBlock,
  MBCHAR const *                          regionBlock,
  aui_Control::ControlActionCallback *    actionFunc,
  void *                                  cookie
) {
  Assert(parentBlock && regionBlock);
  if (!parentBlock || !regionBlock)
    return AUI_ERRCODE_INVALIDPARAM;

  MBCHAR		ldlBlock[k_AUI_LDL_MAXBLOCK + 1];
  sprintf(ldlBlock, "%s.%s", parentBlock, regionBlock);

  return SetActionFuncAndCookie(ldlBlock, actionFunc, cookie);
}


AUI_ERRCODE aui_Ldl::DetachHierarchy(aui_Region *region) {
  if (region) {
    sint32 numChildren = region->NumChildren();

    for (sint32 i = 0; i < numChildren; i++) {
      DetachHierarchy(region->GetChildByIndex(i));
    }

    Remove((void *)region);
  }

  return AUI_ERRCODE_OK;
}

sint32 aui_Ldl::GetIntDependent(const std::string &str) {
  return GetIntDependent(str.c_str());
}

sint32 aui_Ldl::GetIntDependent(MBCHAR const * strPtr) {
  Assert(strPtr != NULL);
  if (!strPtr) return AUI_ERRCODE_INVALIDPARAM;

  sint32 width = g_ui->Width();
  sint32 height = g_ui->Height();

  for (; strPtr; ++strPtr) {
    sint32 w, h, value;
    if (sscanf(strPtr, "%dx%d?%d", &w, &h, &value) != 3)
      if (sscanf(strPtr, "%dX%d?%d", &w, &h, &value) != 3)
        break;

    if (w == width && h == height) return value;

    strPtr = strchr(strPtr, ':');
  }

  return 0;
}

void aui_Ldl::ModifyAttributes(MBCHAR const * ldlBlock, aui_Dimension * dimension) {
  ldl_datablock * format = s_ldl->FindDataBlock(ldlBlock);

  if (format && dimension) {
    sint32 x = dimension->HorizontalPositionData();
    sint32 y = dimension->VerticalPositionData();
    sint32 width = dimension->HorizontalSizeData();
    sint32 height = dimension->VerticalSizeData();

    if (dimension->GetHorizontalPositionType() == AUI_DIMENSION_HPOSITION_ABSOLUTE) {
      if (format->AttributeNameTaken(k_AUI_LDL_HABSPOSITION))
        format->SetValue(k_AUI_LDL_HABSPOSITION, x);
      else
        format->AddAttribute(k_AUI_LDL_HABSPOSITION, x);
    } else {
      if (format->AttributeNameTaken(k_AUI_LDL_HRELPOSITION))
        format->SetValue(k_AUI_LDL_HRELPOSITION, x);
      else
        format->AddAttribute(k_AUI_LDL_HRELPOSITION, x);
    }

    if (dimension->GetVerticalPositionType() == AUI_DIMENSION_VPOSITION_ABSOLUTE) {
      if (format->AttributeNameTaken(k_AUI_LDL_VABSPOSITION))
        format->SetValue(k_AUI_LDL_VABSPOSITION, y);
      else
        format->AddAttribute(k_AUI_LDL_VABSPOSITION, y);
    } else {
      if (format->AttributeNameTaken(k_AUI_LDL_VRELPOSITION))
        format->SetValue(k_AUI_LDL_VRELPOSITION, y);
      else
        format->AddAttribute(k_AUI_LDL_VRELPOSITION, y);
    }

    if (dimension->GetHorizontalSizeType() == AUI_DIMENSION_HSIZE_ABSOLUTE) {
      if (format->AttributeNameTaken(k_AUI_LDL_HABSSIZE))
        format->SetValue(k_AUI_LDL_HABSSIZE, width);
      else
        format->AddAttribute(k_AUI_LDL_HABSSIZE, width);
    } else {
      if (format->AttributeNameTaken(k_AUI_LDL_HRELSIZE))
        format->SetValue(k_AUI_LDL_HRELSIZE, width);
      else
        format->AddAttribute(k_AUI_LDL_HRELSIZE, width);
    }

    if (dimension->GetVerticalSizeType() == AUI_DIMENSION_VSIZE_ABSOLUTE) {
      if (format->AttributeNameTaken(k_AUI_LDL_VABSSIZE))
        format->SetValue(k_AUI_LDL_VABSSIZE, height);
      else
        format->AddAttribute(k_AUI_LDL_VABSSIZE, height);
    } else {
      if (format->AttributeNameTaken(k_AUI_LDL_VRELSIZE))
        format->SetValue(k_AUI_LDL_VRELSIZE, height);
      else
        format->AddAttribute(k_AUI_LDL_VRELSIZE, height);
    }

  }
}

ldl_datablock * aui_Ldl::FindDataBlock(MBCHAR const * ldlBlock) {
  return s_ldl ? s_ldl->FindDataBlock(ldlBlock) : NULL;
}
