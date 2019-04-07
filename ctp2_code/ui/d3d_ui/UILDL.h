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