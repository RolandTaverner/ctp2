#include "ctp/c3.h"

#include "ui/ldl/ldl_list.h"

std::pair<ldl_attribute, bool> LDLAttrListFindAttribute(const ldl_attributelist &attrList, const std::string &name) {
  ldl_attributelist::const_iterator it = std::find_if(std::begin(attrList),
    std::end(attrList),
    [&name](const ldl_attribute &v) { return v.GetName() == name; });

  if (it != attrList.end()) {
    return std::make_pair<>(*it, true);
  }

  return std::make_pair<>(ldl_attribute(), false);
}