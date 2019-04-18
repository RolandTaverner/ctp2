#include "ctp/c3.h"

#include "ui/d3d_ui/Resource/UIImage.h"

namespace ui::d3d {

UIImageResource::UIImageResource() : UIImageResource("") {}

UIImageResource::UIImageResource(const std::string &name) :
  ResourceElement<UIImageResource>(name) {}

bool UIImageResource::LoadImpl(const std::string &fullPath) {
  // TODO: impl
  return false;
}

bool UIImageResource::LoadImpl(const FileInfo &fi) {
  // TODO: impl
  return false;
}

UIImagePtr UIImageResource::GetUIImage() {
  return m_image;
}

} // namespace ui::d3d