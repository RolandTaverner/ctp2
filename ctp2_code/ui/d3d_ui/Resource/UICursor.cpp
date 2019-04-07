#include "ctp/c3.h"

#include "ui/d3d_ui/Resource/UICursor.h"

namespace ui::d3d {

UICursorResource::UICursorResource() : UICursorResource("", "") {}

UICursorResource::UICursorResource(const std::string &name, const std::string &fullPath) :
  ResourceElement<UICursorResource>(name, fullPath) {}

bool UICursorResource::LoadImpl(const std::string &name, const std::string &fullPath) {
  // TODO: impl
  return false;
}

UICursorPtr UICursorResource::GetUICursor() {
  return m_cursor;
}

} // namespace ui::d3d