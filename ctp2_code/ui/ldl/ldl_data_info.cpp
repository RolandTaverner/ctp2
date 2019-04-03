#include "ctp/c3.h"

#include "ui/ldl/ldl_file.hpp"
#include "ui/ldl/ldl_data.hpp"
#include "ui/ldl/ldl_attr.hpp"

int ldl_datablock::GetInt(const char *szName) {
  const ldl_attribute atr = GetAttribute(szName);

  if (!atr.IsEmpty()) {
    if (atr.GetType() == ATTRIBUTE_TYPE_INT)
      return atr.GetIntValue();
    else if (atr.GetType() == ATTRIBUTE_TYPE_DOUBLE)
      return (int)atr.GetFloatValue();
  }
  return 0;
}

double ldl_datablock::GetDouble(const char *szName) {
  const ldl_attribute atr = GetAttribute(szName);

  if (!atr.IsEmpty()) {
    if (atr.GetType() == ATTRIBUTE_TYPE_DOUBLE)
      return atr.GetFloatValue();
  }
  return 0.0;
}

std::string ldl_datablock::GetString(const char *szName) {
  const ldl_attribute atr = GetAttribute(szName);

  if (!atr.IsEmpty()) {
    if (atr.GetType() == ATTRIBUTE_TYPE_STRING)
      return atr.GetStringValue();
  }

  return std::string();
}

bool ldl_datablock::GetBool(const char *szName) {
  const ldl_attribute atr = GetAttribute(szName);

  if (!atr.IsEmpty() && (atr.GetType() == ATTRIBUTE_TYPE_BOOL)) {
    return atr.GetBoolValue();
  }

  return false;
}
