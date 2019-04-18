#pragma once

#include <memory>

#include "ui/d3d_ui/Resource/ResourceElement.h"

namespace ui::d3d {

class UIImage {
public:

};

typedef std::shared_ptr<UIImage> UIImagePtr;

class UIImageResource : public ResourceElement<UIImageResource> {
public:
  UIImageResource();
  UIImageResource(const std::string &name);

  bool LoadImpl(const std::string &fullPath);
  bool LoadImpl(const FileInfo &fi);

  UIImagePtr GetUIImage();

private:
  UIImagePtr m_image;
};


} // namespace ui::d3d
