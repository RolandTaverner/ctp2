#include "ctp/c3.h"

#include "ui/d3d_ui/UIRegion.h"

namespace ui::d3d {

UIRegion::UIRegion(UIRegion::UIRegionPtr parent, unsigned id, 
  const TileEngine::Position &pos, unsigned width, unsigned height) : 
  m_id(id), m_parent(parent) {
  UIRegionPtr parentPtr = m_parent.lock();
  if (!parentPtr) {
    throw std::runtime_error("UIRegion::UIRegion(): Parent should not be NULL");
  }

  m_region = parentPtr->CreateChildUIRegion(pos, width, height);
}

UIRegion::~UIRegion() {}

TileEngine::Region::RegionPtr UIRegion::Region() {
  return m_region;
}

TileEngine::Region::RegionPtr UIRegion::CreateChildUIRegion(const TileEngine::Position & pos, 
  unsigned width, unsigned height) {
  return m_region->AddLayer()->AddChild(pos, width, height);
}

unsigned UIRegion::ID() const {
  return m_id;
}

unsigned UIRegion::Width() const {
  return m_region ? m_region->Width() : 0;
}

unsigned UIRegion::Height() const {
  return m_region ? m_region->Height() : 0;
}

TileEngine::Rect UIRegion::GetRect() const {
  return m_region ? m_region->GetRect() : TileEngine::Rect();
}

UIRegion::UIRegionPtr UIRegion::CreateChild(unsigned id, 
  const TileEngine::Position &pos, unsigned width, unsigned height) {
  UIRegion::UIRegionPtr child(std::make_shared<UIRegion>(shared_from_this(), id, pos, width, height));
  m_children.push_back(child);
  return child;
}

} // namespace ui::d3d