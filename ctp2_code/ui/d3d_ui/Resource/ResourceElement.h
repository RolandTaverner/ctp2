#pragma once

#include <memory>
#include <string>

namespace ui::d3d {

template<typename ImplType> class ResourceElement {
public:
  ResourceElement(const std::string &name, const std::string &fullPath) :
    m_name(name), m_fullPath(fullPath) {
    Load();
  }
  virtual ~ResourceElement() {}

  bool Load() {
    return self()->LoadImpl(Name(), FullPath());
  }

  const std::string &Name() const {
    return m_name;
  }

  const std::string &FullPath() const {
    return m_fullPath;
  }

private:
  ImplType* self() {
    return static_cast<ImplType*>(this);
  }

  std::string m_name;
  std::string m_fullPath;
};

} // namespace ui::d3d