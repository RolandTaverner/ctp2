#pragma once

#include <memory>

#include "D3dTiles/Bitmap.h"
#include "D3dTiles/Geometry.h"
#include "D3dTiles/Primitives/Rectangle.h"

namespace TileEngine {

class TexturedRectangle : public Rectangle {
public:
  typedef std::shared_ptr<TexturedRectangle> Ptr;

  TexturedRectangle();
  TexturedRectangle(unsigned width, unsigned height, const Bitmap::ConstPtr &texture);
  virtual ~TexturedRectangle();

  const Bitmap::ConstPtr &GetTexture() const;
  
private:
  Bitmap::ConstPtr m_texturePtr;
};

} // namespace TileEngine
