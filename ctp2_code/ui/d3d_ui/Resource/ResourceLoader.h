#pragma once

#include <cstdint>
#include <filesystem>
#include <list>
#include <map>
#include <string>

#include <boost/algorithm/string.hpp>

//#include <boost/multi_index_container.hpp>
//#include <boost/multi_index/global_fun.hpp>  
//#include <boost/multi_index/ordered_index.hpp>

#include "ctp/ctp2_utils/c3debug.h"		// Assert
#include "ctp/ctp2_utils/c3files.h"		// C3DIR...
#include "gs/fileio/CivPaths.h"		// g_civPaths

#include "ui/d3d_ui/Resource/ResourceElement.h"

namespace ui::d3d {

//namespace {
//
//struct ByName {};
//
//template<typename ResourceElementImplType>
//const std::string &GetREName(const ResourceElementImplType &ptr) {
//  return ptr.Name();
//}
//
//template<typename ResourceElementImplType>
//const std::string &GetREFullPath(const ResourceElementImplType &ptr) {
//  return ptr.FullPath();
//}
//
//struct ByPath {};
//
//} // namespace

template<typename ResourceElementImplType> 
class ResourceLoader {
private:
  //typedef boost::multi_index_container<
  //  ResourceElementImplType,
  //  boost::multi_index::indexed_by<
  //    boost::multi_index::ordered_unique<
  //      boost::multi_index::tag<ByName>,
  //      boost::multi_index::global_fun<const ResourceElementImplType &, const std::string &, GetREName<ResourceElementImplType> >
  //    >,
  //    boost::multi_index::ordered_unique<
  //      boost::multi_index::tag<ByPath>,
  //      boost::multi_index::global_fun<const ResourceElementImplType &, const std::string &, GetREFullPath<ResourceElementImplType> >
  //    >
  //  >
  //> ResourceElementRegistry;

  //typedef boost::multi_index::nth_index<ResourceElementRegistry, 0>::type ResourceElementRegistryByName;
  //typedef boost::multi_index::nth_index<ResourceElementRegistry, 1>::type ResourceElementRegistryByPath;

  typedef std::map<std::string, ResourceElementImplType> ResourceElementRegistry;
public:
  ResourceLoader() {}
  virtual ~ResourceLoader() {}

  ResourceElementImplType Load(const std::string &resName) {
    Assert(!resName.empty());
    if (resName.empty()) {
      return ResourceElementImplType();
    }

    ResourceElementRegistry::const_iterator it = m_registry.find(resName);
    if (it != m_registry.end()) {
      return it->second;
    }

    const std::string fullPath = FindFile(resName);
    ResourceElementImplType re(resName, fullPath);
    if (re.Load()) {
      m_registry[resName] = re;
      return re;
    }

   return ResourceElementImplType();
  }

  std::string FindFile(const std::string &name) const {
    if (name.find_first_of(':') == std::string::npos && !boost::starts_with(name, FILE_SEP FILE_SEP)) {
      for (auto p : m_paths) {
        const std::filesystem::path fullPath = p / name;
        if (std::filesystem::is_regular_file(fullPath)) {
          return fullPath.string();
        }
      }
    }
    return std::string();
  }

  void AddPath(const std::string &path) {
    m_paths.push_back(std::filesystem::path(path));
  }

private:
  ResourceElementRegistry m_registry;
  std::list<std::filesystem::path> m_paths;
};

} // namespace ui::d3d
/*
template<class T>
class aui_Resource {
public:

  aui_Resource();
  virtual ~aui_Resource();

  T			*Load(const MBCHAR *name, C3DIR dir = C3DIR_DIRECT, uint32 size = 0);

  AUI_ERRCODE	Unload(T *resource);
  AUI_ERRCODE	Unload(const MBCHAR *name);

  AUI_ERRCODE	AddSearchPath(const MBCHAR *path);
  AUI_ERRCODE	RemoveSearchPath(const MBCHAR *path);

  bool FindFile(std::string &fullPath, const std::string &name);

  bool FindFile(MBCHAR *fullPath, const MBCHAR *name);

protected:
  tech_WLList<aui_ResourceElement<T> *>	*m_resourceList;


  static tech_WLList<MBCHAR *>			*m_pathList;

  static sint32							m_resourceRefCount;
};
*/