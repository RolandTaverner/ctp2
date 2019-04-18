#include "ctp/c3.h"

#include <stdexcept>

#include "ui/ldl/ldl_attr.hpp"
#include "ui/ldl/ldl_file.hpp"

#include "ui/d3d_ui/UILDL.h"

namespace ui::d3d {

const char* LDLAttrParent = "parent";
const char* LDLAttrHAnchor = "xanchor";
const char* LDLAttrVAnchor = "yanchor";
const char* LDLAttrHAbsPosition = "xpix";
const char* LDLAttrHRelPosition = "xpct";
const char* LDLAttrVAbsPosition = "ypix";
const char* LDLAttrVRelPosition = "ypct";
const char* LDLAttrHAbsSize = "widthpix";
const char* LDLAttrHRelSize = "widthpct";
const char* LDLAttrVAbsSize = "heightpix";
const char* LDLAttrVRelSize = "heightpct";

class LDL::Impl {
public:
  Impl() {}

  void Load(const std::string &file) {
    m_ldl.ReadData(file.c_str());
  }

  LDLBlockPtr GetBlock(const std::string &name) const {
    auto dataBlockPtr = m_ldl.FindDataBlockPtr(name.c_str());
    if (!dataBlockPtr) {
      return LDLBlockPtr();
    }
    return std::make_shared<LDLBlock>(dataBlockPtr);
  }

  ldl m_ldl;
};

LDL::LDL() : m_pImpl(new LDL::Impl()) {}

LDL::~LDL() {}

void LDL::Load(const std::string &file) {
  m_pImpl->Load(file);
}

LDLBlockPtr LDL::GetBlock(const std::string &name) const {
  return m_pImpl->GetBlock(name);
}

LDLBlock::LDLBlock(::LDLDataBlockPtr dataBlockPtr) : m_dataBlockPtr(dataBlockPtr) {}

LDLBlock::~LDLBlock() {}

const std::string LDLBlock::GetName() const {
  if (!m_dataBlockPtr) {
    throw std::runtime_error("Empty block");
  }
  return m_dataBlockPtr->GetName();
}

const std::string LDLBlock::GetFullName() const {
  if (!m_dataBlockPtr) {
    throw std::runtime_error("Empty block");
  }
  return m_dataBlockPtr->GetFullName();
}

bool LDLBlock::HasAttribute(const std::string &attrName) const {
  return m_dataBlockPtr->GetAttributeType(attrName.c_str()) != ATTRIBUTE_TYPE_UNKNOWN;
}

int LDLBlock::GetInt(const std::string &attrName) const {
  if (!m_dataBlockPtr) {
    throw std::runtime_error("Empty block");
  }
  return m_dataBlockPtr->GetInt(attrName.c_str());
}

double LDLBlock::GetDouble(const std::string &attrName) const {
  if (!m_dataBlockPtr) {
    throw std::runtime_error("Empty block");
  }
  return m_dataBlockPtr->GetDouble(attrName.c_str());
}

std::string LDLBlock::GetString(const std::string &attrName) const {
  if (!m_dataBlockPtr) {
    throw std::runtime_error("Empty block");
  }
  return m_dataBlockPtr->GetString(attrName.c_str());
}

bool LDLBlock::GetBool(const std::string &attrName) const {
  if (!m_dataBlockPtr) {
    throw std::runtime_error("Empty block");
  }
  return m_dataBlockPtr->GetBool(attrName.c_str());
}

namespace impl {

LDLBlockPtr CreateLDLBlock(LDLDataBlockPtr ptr) {
  return std::make_shared<LDLBlock>(ptr);
}

}  // namespace impl

LDLBlock::LDLBlockRange LDLBlock::GetChildren() {
  if (!m_dataBlockPtr) {
    throw std::runtime_error("Empty block");
  }

  return boost::make_iterator_range<>(
    LDLBlockIterator(m_dataBlockPtr->GetChildList().cbegin(), impl::CreateLDLBlock),
    LDLBlockIterator(m_dataBlockPtr->GetChildList().cend(), impl::CreateLDLBlock)
    );
}

} // namespace ui::d3d

