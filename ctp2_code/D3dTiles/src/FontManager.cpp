#include "stdafx.h"

#include <stdexcept>

#include "D3dTiles/FontManager.h"

namespace TileEngine {

FontManager::~FontManager() {}

void FontManager::AddFont(const std::string &fontFace, Font::Ptr font) {
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

Font::Ptr FontManager::GetFont(const std::string &fontFace, FontStyle style) {
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

} // namespace TileEngine