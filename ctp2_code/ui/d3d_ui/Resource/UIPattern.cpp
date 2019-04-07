#include "ctp/c3.h"

#include "ui/d3d_ui/Resource/UIPattern.h"

namespace ui::d3d {

UIPatternResource::UIPatternResource() : UIPatternResource("", "") {}

UIPatternResource::UIPatternResource(const std::string &name, const std::string &fullPath) :
  ResourceElement<UIPatternResource>(name, fullPath) {}

bool UIPatternResource::LoadImpl(const std::string &name, const std::string &fullPath) {
  // TODO: impl
  return false;
}

UIPatternPtr UIPatternResource::GetUIPattern() {
  return m_pattern;
}

} // namespace ui::d3d