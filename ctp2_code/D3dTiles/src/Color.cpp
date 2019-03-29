#include "stdafx.h"

#include <string.h>

#include "D3dTiles/Color.h"

namespace TileEngine {

Color MakeColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) {
  return (std::uint32_t)r | ((std::uint32_t)g << 8) | ((std::uint32_t)b << 16) | ((std::uint32_t)a << 24);
}

Color MakeColor(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
  return MakeColor(r, g, b, 0xFFu);
}

} // namespace TileEngine