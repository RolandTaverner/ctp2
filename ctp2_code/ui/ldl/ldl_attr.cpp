//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
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
// - None
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Initialized local variables. (Sep 9th 2005 Martin Gühmann)
//
//----------------------------------------------------------------------------

#include "ctp/c3.h"

#include <sstream>

#include "ldl_attr.hpp"

std::string ldl_attribute::GetTypeName() const {
  switch (m_type) {
    case ATTRIBUTE_TYPE_BOOL: return "bool";
    case ATTRIBUTE_TYPE_INT:  return "int";
    case ATTRIBUTE_TYPE_DOUBLE: return "double";
    case ATTRIBUTE_TYPE_STRING: return "string";
    default: return "BADTYPE";
  }
}

void ldl_attribute::SetValueInt(int value) {
  m_value = value; m_type = ATTRIBUTE_TYPE_INT;
}

bool ldl_attribute::GetBoolValue() const {
  Assert(m_type == ATTRIBUTE_TYPE_BOOL);
  return std::get<bool>(m_value);
}

int ldl_attribute::GetIntValue() const {
  Assert(m_type == ATTRIBUTE_TYPE_INT);
  return std::get<int>(m_value);
}

double ldl_attribute::GetFloatValue() const {
  Assert(m_type == ATTRIBUTE_TYPE_DOUBLE);
  return std::get<double>(m_value);
}

std::string ldl_attribute::GetStringValue() const {
  Assert(m_type == ATTRIBUTE_TYPE_STRING);
  return std::get<std::string>(m_value);
}

std::string ldl_attribute::GetValueText() const {
  std::ostringstream str;

  switch (m_type) {
    case ATTRIBUTE_TYPE_BOOL:
      str << (GetBoolValue() ? "true" : "false");
      break;
    case ATTRIBUTE_TYPE_INT:
      str << GetIntValue();
      break;
    case ATTRIBUTE_TYPE_DOUBLE:
      str << GetFloatValue();
      break;
    case ATTRIBUTE_TYPE_STRING:
      str << GetStringValue();
      break;
  }

  return str.str();
}
