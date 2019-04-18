#include "stdafx.h"

#include <stdexcept>

#include "DirectXTK/SpriteFont.h"

#include "D3dTiles/D3d/D3dFontManager.h"
#include "D3dTiles/D3d/UTF8Converter.h"

namespace TileEngine::D3d {


class D3dFontManager::ImplType {
public:
  ImplType(ID3D11DevicePtr device) : m_device(device) {}

  ID3D11DevicePtr Device() {
    return m_device;
  }

  D3dFont::Ptr LoadFont(const std::string &file, const std::string &fontFace, unsigned fontSize, FontStyle style) {
    const std::wstring wFile = utf8<std::wstring>(file);

    return std::make_shared<D3dFont>(std::move(std::make_unique<DirectX::SpriteFont>(m_device.GetInterfacePtr(), wFile.c_str(), false)),
      fontFace, fontSize, style);
  }

  ID3D11DevicePtr m_device;
};

D3dFontManager::D3dFontManager(ID3D11DevicePtr device) : m_pImpl(new ImplType(device)) {}

D3dFontManager::~D3dFontManager() {}

D3dFontManager::ImplType &D3dFontManager::Impl() {
  if (!m_pImpl) {
    throw std::runtime_error("");
  }

  return *m_pImpl;
}

Font::Ptr D3dFontManager::LoadFont(const std::string &file, const std::string &fontFace, unsigned fontSize, FontStyle style) {
  D3dFont::Ptr font = Impl().LoadFont(file, fontFace, fontSize, style);
  AddFont(fontFace, font);

  return font;
}

void D3dFontManager::AddFont(const std::string &fontFace, D3dFont::Ptr font) {
  FontRegistryByDesc &byDesc = boost::multi_index::get<0>(m_fonts);
  FontRegistryByDesc::const_iterator it = byDesc.find(GetFontDesc(font));
  if (it != byDesc.end()) {
    throw std::runtime_error("Font " + fontFace + " size " + std::to_string(font->GetFontSize())  +  " style " + std::to_string(font->GetStyle()) + " already exists");
  }

  auto [itIns, ok] = byDesc.insert(font);
  if (!ok) {
    throw std::runtime_error("Font " + fontFace + " size " + std::to_string(font->GetFontSize()) + " style " + std::to_string(font->GetStyle()) + " already exists");
  }
}

D3dFont::Ptr D3dFontManager::GetD3dFont(const std::string &fontFace, unsigned fontSize, FontStyle style) {
  // TODO: get nearest existing size and use font scaling
  FontRegistryByDesc &byDesc = boost::multi_index::get<0>(m_fonts);
  FontRegistryByDesc::const_iterator it = byDesc.find(FontDesc(fontFace, fontSize, style));
  if (it == byDesc.end()) {
    throw std::runtime_error("Font " + fontFace + " size " + std::to_string(fontSize) + " style " + std::to_string(style) + " not found");
  }
  return *it;
}

Font::Ptr D3dFontManager::GetFont(const std::string &fontFace, unsigned fontSize, FontStyle style) {
  return GetD3dFont(fontFace, fontSize, style);
}

} // namespace TileEngine::D3d