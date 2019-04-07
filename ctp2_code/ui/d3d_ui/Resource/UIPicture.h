#pragma once

#include <memory>

#include "ui/d3d_ui/Resource/ResourceElement.h"

namespace ui::d3d {

class UIPicture {
public:

};

typedef std::shared_ptr<UIPicture> UIPicturePtr;

class UIPictureResource : public ResourceElement<UIPictureResource> {
public:
  UIPictureResource();
  UIPictureResource(const std::string &name, const std::string &fullPath);

  bool LoadImpl(const std::string &name, const std::string &fullPath);

  UIPicturePtr GetUIPicture();

private:
  UIPicturePtr m_picture;
};


} // namespace ui::d3d
