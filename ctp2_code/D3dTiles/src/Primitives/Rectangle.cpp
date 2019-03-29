#include "stdafx.h"

#include "D3dTiles/Primitives/Rectangle.h"

namespace TileEngine {

Rectangle::Rectangle() : Rectangle(0, 0) {}

Rectangle::Rectangle(unsigned width, unsigned height) : 
  m_width(width), m_height(height) {}

Rectangle::~Rectangle() {}

unsigned Rectangle::Width() const {
  return m_width;
}

unsigned Rectangle::Height() const {
  return m_height;
}

} // namespace TileEngine