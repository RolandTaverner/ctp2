#include "ctp/c3.h"

#include "ui/d3d_ui/Resource/UIIcon.h"

namespace ui::d3d {

UIIconResource::UIIconResource() : UIIconResource("", "") {}

UIIconResource::UIIconResource(const std::string &name, const std::string &fullPath) :
  ResourceElement<UIIconResource>(name, fullPath) {}

bool UIIconResource::LoadImpl(const std::string &name, const std::string &fullPath) {
  // TODO: impl
  return false;
}

UIIconPtr UIIconResource::GetUIIcon() {
  return m_icon;
}

} // namespace ui::d3d