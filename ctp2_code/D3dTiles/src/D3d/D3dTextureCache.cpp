#include "stdafx.h"

#include "D3d/D3dTextureCache.h"

namespace TileEngine {
namespace D3d {

TextureCache::TextureCache() {}

TextureCache::~TextureCache() {}

ID3D11Texture2DPtr TextureCache::TryGet(const std::string &id) {
  CacheType::const_iterator it = m_cache.find(id);
  if (it != m_cache.end()) {
    return it->second;
  }
  return ID3D11Texture2DPtr();
}

bool TextureCache::Set(const std::string &id, ID3D11Texture2DPtr texture) {
  auto[it, result] = m_cache.try_emplace(id, texture);
  return result;
}

} // namespace D3d
} // namespace TileEngine
