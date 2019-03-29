#include "stdafx.h"

#include "D3dTiles/Primitives/Text.h"

namespace TileEngine {

Text::Text(Font::Ptr font, const std::string &text, Color textColor, Color dropColor,
  const Position &center, const Rect &bounds, TextOpts options) :
  m_font(font), m_text(text), m_textColor(textColor), m_dropColor(dropColor),
  m_center(center), m_bounds(bounds), m_options(options) {}

Text::Text(Font::Ptr font, const std::string &text, Color textColor) :
  Text(font, text, textColor, Color(0u), Position(), Rect(), SIMPLETEXT) {}

Text::Text(Font::Ptr font, const std::string &text, Color textColor, Color dropColor) :
  Text(font, text, textColor, dropColor, Position(), Rect(), DROPTEXT) {}

Text::Text(Font::Ptr font, const std::string &text, Color textColor, const Position &center) :
  Text(font, text, textColor, Color(0u), center, Rect(), CENTERTEXT) {}

Text::Text(Font::Ptr font, const std::string &text, Color textColor, const Rect &bounds) :
  Text(font, text, textColor, Color(0u), Position(), bounds, BOUNDTEXT) {}

Text::~Text() {}

} // namespace TileEngine

