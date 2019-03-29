#pragma once

#include <memory>
#include <vector>

#include "D3dTiles/Geometry.h"
#include "D3dTiles/Region.h"

namespace ui::d3d {

class UIRegion : public std::enable_shared_from_this<UIRegion> {
public:
  typedef std::shared_ptr<UIRegion> UIRegionPtr;
  typedef std::weak_ptr<UIRegion> UIRegionWPtr;
  typedef std::vector<UIRegionPtr> ChildList;

  UIRegion(UIRegionPtr parent, unsigned id, const TileEngine::Position &pos, unsigned width, unsigned height);
  virtual ~UIRegion();

  unsigned ID() const;
  unsigned Width() const;
  unsigned Height() const;
  TileEngine::Rect GetRect() const;
  UIRegionPtr CreateChild(unsigned id, const TileEngine::Position &pos, unsigned width, unsigned height);

protected:
  TileEngine::Region::RegionPtr Region();
  TileEngine::Region::RegionPtr CreateChildUIRegion(const TileEngine::Position &pos, unsigned width, unsigned height);

private:
  unsigned m_id;

  TileEngine::Region::RegionPtr m_region;
  UIRegionWPtr m_parent;
  ChildList m_children;
};

} // namespace ui::d3d