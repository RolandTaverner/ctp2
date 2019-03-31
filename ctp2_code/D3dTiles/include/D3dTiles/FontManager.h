#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include <boost/geometry/arithmetic/arithmetic.hpp>

#include "D3dTiles/Font.h"
#include "D3dTiles/Geometry.h"

namespace TileEngine {

class FontManager {
public:
  typedef std::shared_ptr<FontManager> Ptr;

  virtual ~FontManager();

  virtual Font::Ptr LoadFont(const std::string &file, const std::string &fontFace, unsigned fontSize, FontStyle style) = 0;
  virtual Font::Ptr GetFont(const std::string &fontFace, unsigned fontSize, FontStyle style) = 0;
};

} // name