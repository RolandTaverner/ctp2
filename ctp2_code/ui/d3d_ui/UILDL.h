#pragma once

#include <functional>
#include <memory>
#include <string>

#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include "ui/ldl/ldl_data.hpp"

namespace ui::d3d {

// Common attributes
extern const char* LDLAttrParent;
extern const char* LDLAttrHAnchor;
extern const char* LDLAttrVAnchor;
extern const char* LDLAttrHAbsPosition;
extern const char* LDLAttrHRelPosition;
extern const char* LDLAttrVAbsPosition;
extern const char* LDLAttrVRelPosition;
extern const char* LDLAttrHAbsSize;
extern const char* LDLAttrHRelSize;
extern const char* LDLAttrVAbsSize;
extern const char* LDLAttrVRelSize;


class LDLBlock;
typedef std::shared_ptr<LDLBlock> LDLBlockPtr;


class LDLBlock {
public:
  explicit LDLBlock(::LDLDataBlockPtr dataBlockPtr);
  virtual ~LDLBlock();

  const std::string GetName() const;
  const std::string GetFullName() const;

  bool HasAttribute(const std::string &attrName) const;
  int GetInt(const std::string &attrName) const;
  double GetDouble(const std::string &attrName) const;
  std::string GetString(const std::string &attrName) const;
  bool GetBool(const std::string &attrName) const;
  
  typedef std::function<LDLBlockPtr(::LDLDataBlockPtr)> LdlBlockFromLegacy;
  typedef boost::transform_iterator<
    LdlBlockFromLegacy,
    ::LDLBlockList::const_iterator
  > LDLBlockIterator;
  typedef boost::iterator_range<LDLBlockIterator> LDLBlockRange;

  LDLBlockRange GetChildren();

  template<typename T> bool Is(const std::string &attrName) const {
    return false;
  }

  template<> bool Is<bool>(const std::string &attrName) const {
    return m_dataBlockPtr->GetAttributeType(attrName.c_str()) == ATTRIBUTE_TYPE_BOOL;
  }

  template<> bool Is<std::string>(const std::string &attrName) const {
    return m_dataBlockPtr->GetAttributeType(attrName.c_str()) == ATTRIBUTE_TYPE_STRING;
  }

  template<> bool Is<double>(const std::string &attrName) const {
    return m_dataBlockPtr->GetAttributeType(attrName.c_str()) == ATTRIBUTE_TYPE_DOUBLE;
  }

  template<> bool Is<int>(const std::string &attrName) const {
    return m_dataBlockPtr->GetAttributeType(attrName.c_str()) == ATTRIBUTE_TYPE_INT;
  }
  
  template<typename T> T As(const std::string &attrName) const {
    throw std::runtime_error("Unknown type conversion requested");
  }

  template<> bool As<bool>(const std::string &attrName) const {
    if (!HasAttribute(attrName)) {
      return false;
    } else if (!Is<bool>(attrName)) {
      throw std::runtime_error("Can't convert " + attrName + " to bool");
    }
    return GetBool(attrName.c_str());
  }

  template<> int As<int>(const std::string &attrName) const {
    if (!HasAttribute(attrName)) {
      return 0;
    } else if (!Is<int>(attrName)) {
      throw std::runtime_error("Can't convert " + attrName + " to int");
    }
    return GetInt(attrName.c_str());
  }

  template<> double As<double>(const std::string &attrName) const {
    if (!HasAttribute(attrName)) {
      return 0.0;
    } else if (!Is<double>(attrName)) {
      throw std::runtime_error("Can't convert " + attrName + " to double");
    }
    return GetDouble(attrName.c_str());
  }

  template<> std::string As<std::string>(const std::string &attrName) const {
    if (!HasAttribute(attrName)) {
      return std::string();
    } else if (!Is<std::string>(attrName)) {
      throw std::runtime_error("Can't convert " + attrName + " to std::string");
    }
    return GetString(attrName.c_str());
  }

private:
  LDLDataBlockPtr m_dataBlockPtr;
};


class LDL {
public:
  LDL();
  virtual ~LDL();

  void Load(const std::string &file);
  LDLBlockPtr GetBlock(const std::string &name) const;

private:
  class Impl;
  std::unique_ptr<Impl> m_pImpl;
};

typedef std::shared_ptr<LDL> LDLPtr;

} // namespace ui::d3d