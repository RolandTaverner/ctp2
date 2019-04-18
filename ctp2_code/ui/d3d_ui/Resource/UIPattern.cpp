#include "ctp/c3.h"

#include "ui/d3d_ui/Resource/UIPattern.h"

namespace ui::d3d {

UIPatternResource::UIPatternResource() : UIPatternResource("") {}

UIPatternResource::UIPatternResource(const std::string &name) :
  ResourceElement<UIPatternResource>(name) {}

bool UIPatternResource::LoadImpl(const std::string &fullPath) {
  // TODO: impl
  return false;
}

bool UIPatternResource::LoadImpl(const FileInfo &fi) {
  // TODO: impl
  return false;
}

UIPatternPtr UIPatternResource::GetUIPattern() {
  return m_pattern;
}

} // namespace ui::d3d