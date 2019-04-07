#pragma once

#include <memory>

#include "ui/d3d_ui/Resource/ResourceElement.h"

namespace ui::d3d {

class UICursor {
public:

};

typedef std::shared_ptr<UICursor> UICursorPtr;

class UICursorResource : public ResourceElement<UICursorResource> {
public:
  UICursorResource();
  UICursorResource(const std::string &name, const std::string &fullPath);

  bool LoadImpl(const std::string &name, const std::string &fullPath);

  UICursorPtr GetUICursor();

private:
  UICursorPtr m_cursor;
};


} // namespace ui::d3d
