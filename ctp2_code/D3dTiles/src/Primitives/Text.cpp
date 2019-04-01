#include "stdafx.h"

#include "D3dTiles/Font.h"
#include "D3dTiles/Primitives/Text.h"

namespace TileEngine {

Text::Text(FontManager::Ptr fontManager, FontDesc font, const std::string &text, Color textColor, Color dropColor,
  const Position &center, const Rect &bounds, TextOpts options, float scale) :
  m_fontManager(fontManager), m_font(font), m_text(text), m_textColor(textColor), m_dropColor(dropColor),
  m_center(center), m_bounds(bounds), m_options(options), m_scale(scale) {}

Text::Text(FontManager::Ptr fontManager, FontDesc font, const std::string &text, Color textColor, float scale) :
  Text(fontManager, font, text, textColor, Color(0u), Position(), Rect(), SIMPLETEXT, scale) {}

Text::Text(FontManager::Ptr fontManager, FontDesc font, const std::string &text, Color textColor, Color dropColor, float scale) :
  Text(fontManager, font, text, textColor, dropColor, Position(), Rect(), DROPTEXT, scale) {}

Text::Text(FontManager::Ptr fontManager, FontDesc font, const std::string &text, Color textColor, const Position &center, float scale) :
  Text(fontManager, font, text, textColor, Color(0u), center, Rect(), CENTERTEXT, scale) {}

Text::Text(FontManager::Ptr fontManager, FontDesc font, const std::string &text, Color textColor, const Rect &bounds, float scale) :
  Text(fontManager, font, text, textColor, Color(0u), Position(), bounds, BOUNDTEXT, scale) {}

const std::string Text::GetFontFace() const {
  return std::get<0>(m_font);
}

unsigned Text::GetFontSize() const {
  return std::get<1>(m_font);
}

FontStyle Text::GetFontStyle() const {
  return std::get<2>(m_font);
}

const std::string Text::GetText() const {
  return m_text;
}

Color Text::GetTextColor() const {
  return m_textColor;
}

float Text::GetScale() const {
  return m_scale;
}

unsigned Text::Width() const {
  Font::Ptr font = m_fontManager->GetFont(GetFontFace(), GetFontSize(), GetFontStyle());
  return font->GetTextWidth(m_text)*m_scale + 0.5f;
}

unsigned Text::Height() const {
  Font::Ptr font = m_fontManager->GetFont(GetFontFace(), GetFontSize(), GetFontStyle());
  return font->GetTextHeight(m_text)*m_scale + 0.5f;
}

Text::~Text() {}

} // namespace TileEngine

