#pragma once

#include <memory>
#include <string>
#include <tuple>

#include <boost/geometry/arithmetic/arithmetic.hpp>

#include "D3dTiles/Geometry.h"

namespace TileEngine {

enum FontStyle : std::uint32_t {
  FONTSTYLE_REGULAR = 0,
  FONTSTYLE_BOLD = 0x1,
};

typedef std::tuple<std::string, unsigned, FontStyle> FontDesc;

class Font {
public:
  typedef std::shared_ptr<Font> Ptr;
  
  Font(const std::string &fontFace, unsigned fontSize, FontStyle style);
  virtual ~Font();

  const std::string &GetFontFace() const;
  unsigned GetFontSize() const;
  FontStyle GetStyle() const;

  virtual Rect GetTextRect(const std::string &text) const = 0; // textutils_GetBounds
  virtual Rect GetTextRect(const std::string &text, const Position &pos) const = 0; // textutils_GetBounds
  virtual unsigned GetTextHeight(const std::string &text) const = 0; // textutils_GetHeight
  virtual unsigned GetTextWidth(const std::string &text) const = 0; // textutils_GetWidth
  virtual unsigned GetFontHeight() = 0; // textutils_GetFontHeight

private:
  std::string m_fontFace;
  unsigned m_fontSize;
  FontStyle m_style;
};

} // namespace TileEngine