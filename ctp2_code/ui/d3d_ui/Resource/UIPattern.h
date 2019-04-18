#pragma once

#include <memory>

#include "ui/d3d_ui/Resource/ResourceElement.h"

namespace ui::d3d {

class UIPattern {
public:

};

typedef std::shared_ptr<UIPattern> UIPatternPtr;

class UIPatternResource : public ResourceElement<UIPatternResource> {
public:
  UIPatternResource();
  explicit UIPatternResource(const std::string &name);

  bool LoadImpl(const std::string &fullPath);
  bool LoadImpl(const FileInfo &fi);

  UIPatternPtr GetUIPattern();

private:
  UIPatternPtr m_pattern;
};


} // namespace ui::d3d
