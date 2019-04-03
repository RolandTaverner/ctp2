/*******************************************************************************

  $Workfile:: ldl_attr.hpp                                                   $
    $Author$
      $Date$
  $Rev$
   $Archive:: /LDL/test/ldl_attr.hpp                                         $

*******************************************************************************/

#ifndef LDL_ATTR_HPP
#define LDL_ATTR_HPP

#include <string>
#include <variant>

enum ATTRIBUTE_TYPE {
  ATTRIBUTE_TYPE_UNKNOWN,
  ATTRIBUTE_TYPE_INT,
  ATTRIBUTE_TYPE_DOUBLE,
  ATTRIBUTE_TYPE_STRING,
  ATTRIBUTE_TYPE_BOOL,
};

typedef std::variant<bool, int, double, std::string> ldl_attribute_value;

class ldl_attribute {
public:
  ldl_attribute() : m_type(ATTRIBUTE_TYPE_UNKNOWN) {}
  ldl_attribute(const std::string &name, ATTRIBUTE_TYPE type, const ldl_attribute_value &value) :
    m_name(name), m_type(type), m_value(value) {}
  ~ldl_attribute() {}

  bool IsEmpty() const { return m_type == ATTRIBUTE_TYPE_UNKNOWN; }
  ATTRIBUTE_TYPE GetType() const { return m_type; }
  const std::string &GetName() const { return m_name; }

  std::string GetTypeName() const;
  void SetValueInt(int value);

  bool GetBoolValue() const;
  int GetIntValue() const;
  double GetFloatValue() const;
  std::string GetStringValue() const;
  std::string GetValueText() const;

private:
  std::string m_name;
  ATTRIBUTE_TYPE m_type;
  ldl_attribute_value m_value;
};

#endif //LDL_ATTR_HPP
