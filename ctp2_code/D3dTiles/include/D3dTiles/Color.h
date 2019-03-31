#pragma once

#include <cstdint>

namespace TileEngine {

typedef std::uint32_t Color;

Color MakeColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

Color MakeColor(std::uint8_t r, std::uint8_t g, std::uint8_t b);

template<typename Component> std::uint8_t GetComponent(Color c) {
  return c >> Component::shift & 0xFFu;
}

template<unsigned Shift>
struct ComponentShift {
  static const unsigned shift = Shift;
};

struct Red : ComponentShift<0> {};
struct Green : ComponentShift<8> {};
struct Blue : ComponentShift<16> {};
struct Alpha : ComponentShift<24> {};

} // namespace TileEngine