#include "stdafx.h"

#include "D3dTiles/Font.h"

namespace TileEngine {

Font::Font(const std::string &fontFace, unsigned fontSize, FontStyle style) :
  m_fontFace(fontFace), m_fontSize(fontSize), m_style(style) {}

Font::~Font() {}

const std::string & Font::GetFontFace() const {
  return m_fontFace;
}

unsigned Font::GetFontSize() const {
  return m_fontSize;
}

FontStyle Font::GetStyle() const {
  return m_style;
}

} // namespace TileEngine