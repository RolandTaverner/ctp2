#pragma once

#include <memory>

#include "ui/d3d_ui/Resource/ResourceElement.h"

namespace ui::d3d {

class UIIcon {
public:

};

typedef std::shared_ptr<UIIcon> UIIconPtr;

class UIIconResource : public ResourceElement<UIIconResource> {
public:
  UIIconResource();
  UIIconResource(const std::string &name);

  bool LoadImpl(const std::string &fullPath);
  bool LoadImpl(const FileInfo &fi);

  UIIconPtr GetUIIcon();

private:
  UIIconPtr m_icon;
};

} // namespace ui::d3d