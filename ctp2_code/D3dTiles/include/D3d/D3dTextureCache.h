#pragma once

#include <map>
#include <string>

#include "D3d/D3dInterfaces.h"

namespace TileEngine {
namespace D3d {

class TextureCache {
public:
  TextureCache();
  virtual ~TextureCache();

  ID3D11Texture2DPtr TryGet(const std::string &id);
  bool Set(const std::string &id, ID3D11Texture2DPtr texture);

private:
  typedef std::map<std::string, ID3D11Texture2DPtr> CacheType;

  CacheType m_cache;
};

} // namespace D3d
} // namespace TileEngine
