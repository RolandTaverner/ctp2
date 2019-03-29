#include "stdafx.h"

#include "D3dTiles/Primitives/ColoredRectangle.h"

namespace TileEngine {

ColoredRectangle::ColoredRectangle() : m_color(0) {}

ColoredRectangle::ColoredRectangle(unsigned width, unsigned height, Color c) :
  Rectangle(width, height), m_color(c) {}

ColoredRectangle::~ColoredRectangle() {}

Color ColoredRectangle::GetColor() const {
  return m_color;
}

void ColoredRectangle::SetColor(Color c) {
  m_color = c;
}

} // namespace TileEngine