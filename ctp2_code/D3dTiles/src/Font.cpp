#include "stdafx.h"

#include "D3dTiles/Font.h"

namespace TileEngine {

Font::Font(const std::string & fontFace, FontStyle style) :
  m_fontFace(fontFace), m_style(style) {}

Font::~Font() {}

const std::string & Font::GetFontFace() const {
  return m_fontFace;
}

FontStyle Font::GetStyle() const {
  return m_style;
}

} // namespace TileEngine