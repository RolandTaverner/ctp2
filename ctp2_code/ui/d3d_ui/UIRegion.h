#pragma once

#include <memory>
#include <string>
#include <vector>

#include "D3dTiles/Geometry.h"
#include "D3dTiles/Region.h"

#include "ui/d3d_ui/UIDimensioner.h"
#include "ui/d3d_ui/UIRegionFwd.h"
#include "ui/d3d_ui/UILDL.h"

namespace ui::d3d {

class UIRegion : public std::enable_shared_from_this<UIRegion> {
public:
  typedef std::vector<UIRegionPtr> ChildList;

  UIRegion(UIRegionPtr parent, unsigned id, const TileEngine::Position &pos, unsigned width, unsigned height);
  UIRegion(UIRegionPtr parent, unsigned id, LDLBlockPtr ldlBlock);
  virtual ~UIRegion();

  unsigned ID() const;
  const std::string &GetLDLBlockName() const;
  unsigned Width() const;
  unsigned Height() const;
  TileEngine::Rect GetRect() const;
  virtual void MoveTo(unsigned x, unsigned y);
  virtual void MoveBy(int dx, int dy);
  virtual void Resize(unsigned width, unsigned height);

  UIRegionPtr CreateChild(unsigned id, const TileEngine::Position &pos, unsigned width, unsigned height);

protected:
  TileEngine::Region::Ptr Region();
  TileEngine::Region::Ptr CreateChildUIRegion(const TileEngine::Position &pos, unsigned width, unsigned height);

private:
  void InitFromLDL();

private:
  unsigned m_id;
  UIDimensioner m_dim;
  LDLBlockPtr m_ldlBlock;
  TileEngine::Region::Ptr m_region;
  UIRegionWPtr m_parent;
  ChildList m_children;
  bool m_blind;
};

} // namespace ui::d3d