#pragma once

#include <memory>
#include <string>

#include <boost/geometry/arithmetic/arithmetic.hpp>

#include "D3dTiles/Geometry.h"

namespace TileEngine {

class Font {
public:
  typedef std::shared_ptr<Font> Ptr;

  virtual ~Font();

  virtual Rect GetTextRect(const std::string &text) = 0; // textutils_GetBounds
  virtual unsigned GetTextHeight(const std::string &text) = 0; // textutils_GetHeight
  virtual unsigned GetTextWidth(const std::string &text) = 0; // textutils_GetWidth
  virtual unsigned GetFontHeight() = 0; // textutils_GetFontHeight
};

} // namespace TileEngine