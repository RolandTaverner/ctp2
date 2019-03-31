#pragma once

#include <memory>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/global_fun.hpp>  
#include <boost/multi_index/ordered_index.hpp>

#include <DirectXMath.h>

#include "D3dTiles/FontManager.h"
#include "D3dTiles/D3d/D3dFont.h"
#include "D3dTiles/D3d/D3dInterfaces.h"

namespace TileEngine::D3d {

namespace {
std::string GetFontFace(const D3dFont::Ptr &font) {
  return font->GetFontFace();
}
unsigned GetFontSize(const D3dFont::Ptr &font) {
  return font->GetFontSize();
}
FontStyle GetFontStyle(const D3dFont::Ptr &font) {
  return font->GetStyle();
}
FontDesc GetFontDesc(const D3dFont::Ptr &font) {
  return FontDesc(font->GetFontFace(), font->GetFontSize(), font->GetStyle());
}
}

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
  Font::Ptr LoadFont(const std::string &file, const std::string &fontFace, unsigned fontSize, FontStyle style) override;
  Font::Ptr GetFont(const std::string &fontFace, unsigned fontSize, FontStyle style) override;

  D3dFont::Ptr GetD3dFont(const std::string &fontFace, unsigned fontSize, FontStyle style);

private:
  void AddFont(const std::string &fontFace, D3dFont::Ptr font);
  ImplType &Impl();

private:

  struct FontFaceSizeStyleKey : boost::multi_index::composite_key<
    D3dFont::Ptr,
    boost::multi_index::global_fun<const D3dFont::Ptr&, std::string, GetFontFace>,
    boost::multi_index::global_fun<const D3dFont::Ptr&, unsigned, GetFontSize>,
    boost::multi_index::global_fun<const D3dFont::Ptr&, FontStyle, GetFontStyle>
  > {};

  typedef boost::multi_index_container<
    D3dFont::Ptr,
    boost::multi_index::indexed_by<
      boost::multi_index::ordered_unique<
        boost::multi_index::global_fun<const D3dFont::Ptr&, FontDesc, GetFontDesc>
      >,
      boost::multi_index::ordered_non_unique<FontFaceSizeStyleKey>
    >
  > FontRegistry;

  typedef boost::multi_index::nth_index<FontRegistry, 0>::type FontRegistryByDesc;
  typedef boost::multi_index::nth_index<FontRegistry, 1>::type FontRegistryByFaceSizeStyle;

  FontRegistry m_fonts;
  std::unique_ptr<ImplType> m_pImpl;
};

} // namespace TileEngine::D3d 