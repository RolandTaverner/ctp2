#ifndef LDL_LIST_H__
#define LDL_LIST_H__

#include <algorithm>
#include <list>
#include <utility>

#include "ui/ldl/ldl_attr.hpp"

typedef std::list<ldl_attribute> ldl_attributelist;

std::pair<ldl_attribute, bool> LDLAttrListFindAttribute(const ldl_attributelist &attrList, const std::string &name);


#endif
