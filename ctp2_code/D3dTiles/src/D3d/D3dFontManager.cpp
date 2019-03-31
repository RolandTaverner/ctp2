#include "stdafx.h"

#include <codecvt>
#include <stdexcept>

#include "DirectXTK/SpriteFont.h"

#include "D3dTiles/D3d/D3dFontManager.h"

namespace TileEngine::D3d {

class D3dFontManager::ImplType {
public:
  ImplType(ID3D11DevicePtr device) : m_device(device) {}

  ID3D11DevicePtr Device() {
    return m_device;
  }

  D3dFont::Ptr LoadFont(const std::string &file, const std::string &fontFace, FontStyle style) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
    const std::wstring wFile(convert.from_bytes(file));

    return std::make_shared<D3dFont>(std::move(std::make_unique<DirectX::SpriteFont>(m_device.GetInterfacePtr(), wFile.c_str(), false)),
      fontFace, style);
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

Font::Ptr D3dFontManager::LoadFont(const std::string &file, const std::string &fontFace, FontStyle style) {
  D3dFont::Ptr font = Impl().LoadFont(file, fontFace, style);
  AddFont(fontFace, font);

  return font;
}

void D3dFontManager::AddFont(const std::string &fontFace, D3dFont::Ptr font) {
  auto it = m_fonts.find(fontFace);
  if (it == m_fonts.end()) {
    FontStyleMap fsMap;
    fsMap[font->GetStyle()] = font;
    m_fonts[fontFace] = fsMap;
    return;
  }
  FontStyleMap &fsMap = it->second;
  auto[existingIt, ok] = fsMap.try_emplace(font->GetStyle(), font);
  if (!ok) {
    throw std::runtime_error("Font " + fontFace + " with style " + std::to_string(font->GetStyle()) + " already exists");
  }
}

D3dFont::Ptr D3dFontManager::GetD3dFont(const std::string &fontFace, FontStyle style) {
  auto itFontFamily = m_fonts.find(fontFace);
  if (itFontFamily == m_fonts.end()) {
    throw std::runtime_error("Font " + fontFace + " not found");
  }

  auto itFont = itFontFamily->second.find(style);
  if (itFont == itFontFamily->second.end()) {
    throw std::runtime_error("Font " + fontFace + " with style " + std::to_string(style) + " not found");
  }

  return itFont->second;
}
Font::Ptr D3dFontManager::GetFont(const std::string &fontFace, FontStyle style) {
  return GetD3dFont(fontFace, style);
}

} // namespace TileEngine::D3d