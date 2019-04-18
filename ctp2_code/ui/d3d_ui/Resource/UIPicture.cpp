#include "ctp/c3.h"

#include "ui/d3d_ui/Resource/UIPicture.h"

namespace ui::d3d {

UIPictureResource::UIPictureResource() : UIPictureResource("") {}

UIPictureResource::UIPictureResource(const std::string &name) :
  ResourceElement<UIPictureResource>(name) {}

bool UIPictureResource::LoadImpl(const std::string &fullPath) {
  // TODO: impl
  return false;
}

bool UIPictureResource::LoadImpl(const FileInfo &fi) {
  // TODO: impl
  return false;
}

UIPicturePtr UIPictureResource::GetUIPicture() {
  return m_picture;
}

} // namespace ui::d3d