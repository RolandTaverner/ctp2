#pragma once

#include <memory>
#include <string>

#include "ui/d3d_ui/Resource/ResourceFileManager.h"

namespace ui::d3d {

template<typename ImplType> class ResourceElement {
public:
  explicit ResourceElement(const std::string &name) :
    m_name(name) {
  }
  virtual ~ResourceElement() {}

  bool Load(const std::string &fullPath) {
    return self()->LoadImpl(fullPath);
  }

  bool Load(const FileInfo &fi) {
    return self()->LoadImpl(fi);
  }

  const std::string &Name() const {
    return m_name;
  }

private:
  ImplType* self() {
    return static_cast<ImplType*>(this);
  }

  std::string m_name;
};

} // namespace ui::d3d