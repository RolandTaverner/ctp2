#include "ctp/c3.h"

#include "ui/d3d_ui/Resource/UIIcon.h"

namespace ui::d3d {

UIIconResource::UIIconResource() : UIIconResource("") {}

UIIconResource::UIIconResource(const std::string &name) :
  ResourceElement<UIIconResource>(name) {}

bool UIIconResource::LoadImpl(const std::string &fullPath) {
  // TODO: impl
  return false;
}

bool UIIconResource::LoadImpl(const FileInfo &fi) {
  // TODO: impl
  return false;
}

UIIconPtr UIIconResource::GetUIIcon() {
  return m_icon;
}

} // namespace ui::d3d