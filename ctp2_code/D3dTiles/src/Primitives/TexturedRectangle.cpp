#include "stdafx.h"

#include "D3dTiles/Primitives/TexturedRectangle.h"

namespace TileEngine {

TexturedRectangle::TexturedRectangle() {}

TexturedRectangle::TexturedRectangle(unsigned width, unsigned height, const Bitmap::ConstPtr &texture) :
  Rectangle(width, height), m_texturePtr(texture) {}

TexturedRectangle::~TexturedRectangle() {}

const Bitmap::ConstPtr &TexturedRectangle::GetTexture() const {
  return m_texturePtr;
}

} // namespace TileEngine