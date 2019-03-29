#pragma once

#include <memory>

#include "D3dTiles/Color.h"
#include "D3dTiles/Geometry.h"
#include "D3dTiles/Primitives/Rectangle.h"

namespace TileEngine {

class ColoredRectangle : public Rectangle {
public:
  typedef std::shared_ptr<ColoredRectangle> Ptr;

  ColoredRectangle();
  ColoredRectangle(unsigned width, unsigned height, Color c);
  virtual ~ColoredRectangle();

  Color GetColor() const;
  void SetColor(Color c);

private:
  Color m_color;
};

} // namespace TileEngine
