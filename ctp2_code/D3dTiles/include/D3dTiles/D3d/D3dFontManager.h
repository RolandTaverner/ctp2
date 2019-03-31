#pragma once

#include <memory>

#include <DirectXMath.h>

#include "D3dTiles/FontManager.h"
#include "D3dTiles/D3d/D3dFont.h"
#include "D3dTiles/D3d/D3dInterfaces.h"

namespace TileEngine::D3d {

class D3dFontManager : public TileEngine::FontManager {
private:
  class ImplType;
public:
  typedef std::shared_ptr<D3dFontManager> Ptr;

  explicit D3dFontManager(ID3D11DevicePtr device);
  D3dFontManager(const D3dFontManager&) = delete;
  D3dFontManager &operator=(const D3dFontManager&) = delete;
  virtual ~D3dFontManager();

  // FontManager
  Font::Ptr LoadFont(const std::string &file, const std::string &fontFace, FontStyle style) override;
  Font::Ptr GetFont(const std::string &fontFace, FontStyle style) override;

  D3dFont::Ptr GetD3dFont(const std::string &fontFace, FontStyle style);

private:
  void AddFont(const std::string &fontFace, D3dFont::Ptr font);
  ImplType &Impl();

private:
  typedef std::map<FontStyle, D3dFont::Ptr> FontStyleMap;
  typedef std::map<std::string, FontStyleMap> FontMap;

  FontMap m_fonts;
  std::unique_ptr<ImplType> m_pImpl;
};

} // namespace TileEngine::D3d 