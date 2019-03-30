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

  virtual Font::Ptr LoadFont(const std::string &file, const std::string &fontFace, FontStyle style) = 0;
  Font::Ptr GetFont(const std::string &fontFace, FontStyle style);

protected:
  void AddFont(const std::string &fontFace, Font::Ptr font);

private:
  typedef std::map<FontStyle, Font::Ptr> FontStyleMap;
  typedef std::map<std::string, FontStyleMap> FontMap;

  FontMap m_fonts;
};

} // name