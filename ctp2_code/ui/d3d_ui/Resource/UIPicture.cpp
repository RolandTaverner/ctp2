#include "ctp/c3.h"

#include "ui/d3d_ui/Resource/UIPicture.h"

namespace ui::d3d {

UIPictureResource::UIPictureResource() : UIPictureResource("", "") {}

UIPictureResource::UIPictureResource(const std::string &name, const std::string &fullPath) :
  ResourceElement<UIPictureResource>(name, fullPath) {}

bool UIPictureResource::LoadImpl(const std::string &name, const std::string &fullPath) {
  // TODO: impl
  return false;
}

UIPicturePtr UIPictureResource::GetUIPicture() {
  return m_picture;
}

} // namespace ui::d3d