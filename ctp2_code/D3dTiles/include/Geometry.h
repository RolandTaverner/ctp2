#pragma once

#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

namespace TileEngine {

  typedef boost::geometry::model::d2::point_xy<int> Position;
  typedef boost::geometry::model::box<Position> Rect;

} // namespace TileEngine

