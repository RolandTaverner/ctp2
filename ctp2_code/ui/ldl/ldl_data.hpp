//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ header file
// Description  :
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
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
//
//----------------------------------------------------------------------------

#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif

#ifndef LDL_DATA_HPP
#define LDL_DATA_HPP

#include <list>
#include <memory>
#include <string>

class ldl_datablock;

typedef std::shared_ptr<ldl_datablock> LDLBlockPtr;
typedef std::weak_ptr<ldl_datablock> LDLBlockWeakPtr;
typedef std::list<LDLBlockPtr> LDLBlockList;

#include "ctp/ctp2_utils/pointerlist.h"
#include "ui/ldl/ldl_list.h"
#include "ui/ldl/ldl_attr.hpp"

class ldl;
class ldl_hash;
class ldl_attribute;
class ldl_filelist;

class ldl_datablock : public std::enable_shared_from_this<ldl_datablock> {
private:
  LDLBlockList m_templates;
  LDLBlockList m_children;
  ldl_attributelist m_attributes;
  LDLBlockWeakPtr m_parent;
  std::string m_name;


public:
  ldl_datablock(PointerList<char> *templateNames);
  ldl_datablock(const ldl_datablock &rhs);
  ldl_datablock(sint32 hash);
  ldl_datablock(ldl *theLdl, char const * name);
  virtual ~ldl_datablock();

  const std::string &GetName() const { return m_name; }
  LDLBlockList &GetChildList() { return m_children; }
  const ldl_attributelist &GetAttributes() const { return m_attributes; }

  // Add a child block
  void AddChild(LDLBlockPtr child) {
    m_children.push_back(child);
    child->m_parent = shared_from_this();
  }

  // Add a single attribute
  void AddAttribute(const ldl_attribute &attr) {
    m_attributes.push_back(attr);
  }

  void AddAttribute(char *name, char *str) {
    m_attributes.push_back(ldl_attribute(name, ATTRIBUTE_TYPE_STRING, std::string(str)));
  }

  void AddAttribute(char *name, int value) {
    m_attributes.push_back(ldl_attribute(name, ATTRIBUTE_TYPE_INT, value));
  }

  void CopyAttributesFrom(LDLBlockPtr templ);

  void AddTemplateChildren();
  void AddTemplateChildrenTo(LDLBlockPtr block);

  void Dump(sint32 indent);

  std::string GetFullName() const;

public:

  // returns the attribute for the given name, error only if bMustExist = TRUE
  const ldl_attribute GetAttribute(const char *szName) const;

  // returns the attribute value or 0 if the attribute can't be found.
  int GetAttributeType(const char *szName) const;

  // function used for templates -only-
  bool AttributeNameTaken(char *szName);
  void SetValue(char *name, int value);

  /* ------ functions found in ldl_data_info.cpp ------ */
    // used to return datablock values.
  int     GetInt(const char *szName = "int");
  double  GetDouble(const char *szName = "double");
  bool    GetBool(const char *szName = "bool");
  //	RECT GetRect(const char *x1 = "rect_x1", const char *y1 = "rect_y1",
  //				 const char *x2 = "rect_x2", const char *y2 = "rect_y2");
  //	POINT GetPoint(const char *x1 = "point_x", const char *y1 = "point_y");
  std::string GetString(const char *szName);
};




#endif //LDL_DATA_HPP
