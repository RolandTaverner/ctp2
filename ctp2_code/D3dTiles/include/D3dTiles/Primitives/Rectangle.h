#pragma once

#include "D3dTiles/Geometry.h"
#include "D3dTiles/Primitives/Primitive.h"

namespace TileEngine {

class Rectangle : public Primitive {
public:
  Rectangle();
  Rectangle(unsigned width, unsigned height);
  virtual ~Rectangle();

  unsigned Width() const override;
  unsigned Height() const override;

private:
  unsigned m_width, m_height;
};

} // namespace TileEngine
