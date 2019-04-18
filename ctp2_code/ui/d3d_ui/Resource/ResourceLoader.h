#pragma once

#include <algorithm>
#include <cstdint>
#include <filesystem> 
#include <list>
#include <map>
#include <string>

#include <boost/algorithm/string.hpp>

#include "ui/d3d_ui/Resource/ResourceElement.h"
#include "ui/d3d_ui/Resource/ResourceFileManager.h"

namespace ui::d3d {

typedef std::list<PathContentTag> ResourcePathContentTags;

template<typename ResourceElementImplType> 
class ResourceLoader {
private:
  typedef std::map<std::string, ResourceElementImplType> ResourceElementRegistry;

public:
  ResourceLoader() {}
  ResourceLoader(ResourceFileManagerPtr fileManager, const ResourcePathContentTags &contentTags) :
    m_fileManager(fileManager) {
    std::copy(contentTags.begin(), contentTags.end(), std::back_inserter(m_contentTags));
  }
  virtual ~ResourceLoader() {}

  void SetFileManager(ResourceFileManagerPtr fileManager) {
    m_registry.clear();
    m_fileManager = fileManager;
  }

  ResourceElementImplType Load(const std::string &resName, PathContentTag tag = PATH_DIRECT) {
    Assert(!resName.empty());
    if (resName.empty()) {
      return ResourceElementImplType();
    }

    ResourceElementRegistry::const_iterator it = m_registry.find(resName);
    if (it != m_registry.end()) {
      return it->second;
    }

    ResourceElementImplType re(resName);
    bool loaded = false;
    
    ResourcePathContentTags tryContentTags;
    if (tag == PATH_DIRECT) {
      std::copy(m_contentTags.begin(), m_contentTags.end(), std::back_inserter(tryContentTags));
    } else {
      tryContentTags.push_back(tag);
    }

    for (PathContentTag tag : tryContentTags) {
      FileInfo fi = m_fileManager->FindFile(resName, tag);
      if (fi) {
        loaded = re.Load(fi);
        break;
      }
    }

    if (!loaded) {
      return ResourceElementImplType();
    }

    m_registry[resName] = re;
    return re;
  }

private:
  ResourceFileManagerPtr m_fileManager;
  ResourceElementRegistry m_registry;
  ResourcePathContentTags m_contentTags;
};

} // namespace ui::d3d
