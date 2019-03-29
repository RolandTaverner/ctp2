#pragma once

#include "D3dTiles/Geometry.h"

namespace TileEngine {

class Primitive {
public:
  typedef std::shared_ptr<Primitive> PrimitivePtr;

  virtual ~Primitive();
  virtual unsigned Width() const = 0;
  virtual unsigned Height() const = 0;
};

} // namespace TileEngine
