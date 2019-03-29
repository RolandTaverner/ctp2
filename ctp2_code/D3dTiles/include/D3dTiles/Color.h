#pragma once

#include <cstdint>

namespace TileEngine {

typedef std::uint32_t Color;

Color MakeColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

Color MakeColor(std::uint8_t r, std::uint8_t g, std::uint8_t b);

} // namespace TileEngine