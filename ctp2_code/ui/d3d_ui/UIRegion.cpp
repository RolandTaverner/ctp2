#include "ctp/c3.h"

#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "ui/aui_common/aui_ldl.h"
#include "ui/ldl/ldl_attr.hpp"
#include "ui/ldl/ldl_data.hpp"

#include "ui/d3d_ui/UIRegion.h"

namespace ba = boost::algorithm;

namespace ui::d3d {

static const char *LDLAttrBlindness = "mouseblind";

UIRegion::UIRegion(UIRegionPtr parent, unsigned id,
  const TileEngine::Position &pos, unsigned width, unsigned height) :
  m_id(id),
  m_region(std::make_shared<TileEngine::Region>(parent ? parent->Region() : TileEngine::Region::Ptr(), id, pos, width, height, true, true)),
  m_dim(parent), m_blind(false) {
}

UIRegion::UIRegion(UIRegionPtr parent, unsigned id, LDLBlockPtr ldlBlock) :
  m_id(id), m_parent(parent), m_ldlBlock(ldlBlock), m_dim(parent), m_blind(false) {
  InitFromLDL();
}

UIRegion::~UIRegion() {}

void UIRegion::InitFromLDL() {
  Assert(m_ldlBlock);

  if (m_ldlBlock->Is<bool>(LDLAttrBlindness)) {
    m_blind = m_ldlBlock->As<bool>(LDLAttrBlindness);
  }

  if (const std::string horizontalAnchor = m_ldlBlock->GetString(LDLAttrHAnchor); 
    !horizontalAnchor.empty()) {
    if (ba::iequals(horizontalAnchor, "right")) {
      m_dim.AnchorRight();
    } else if (ba::iequals(horizontalAnchor, "center")) {
      m_dim.AnchorHorizontalCenter();
    } else {
      m_dim.AnchorLeft();
    }
  } else {
    m_dim.AnchorLeft();
  }

  if (const std::string verticalAnchor = m_ldlBlock->GetString(LDLAttrVAnchor);
    !verticalAnchor.empty()) {
    if (ba::iequals(verticalAnchor, "bottom")) {
      m_dim.AnchorBottom();
    } else if (ba::iequals(verticalAnchor, "center")) {
      m_dim.AnchorVerticalCenter();
    } else {
      m_dim.AnchorTop();
    }
  } else {
    m_dim.AnchorTop();
  }

  if (m_ldlBlock->Is<int>(LDLAttrHRelPosition)) {
    m_dim.SetHorizontalPosition(m_ldlBlock->As<int>(LDLAttrHRelPosition));
    m_dim.AbsoluteHorizontalPosition(false);
  } else {
    m_dim.SetHorizontalPosition(m_ldlBlock->As<int>(LDLAttrHAbsPosition));
    m_dim.AbsoluteHorizontalPosition(true);
  }

  if (m_ldlBlock->Is<int>(LDLAttrVRelPosition)) {
    m_dim.SetVerticalPosition(m_ldlBlock->As<int>(LDLAttrVRelPosition));
    m_dim.AbsoluteVerticalPosition(false);
  } else {
    m_dim.SetVerticalPosition(m_ldlBlock->As<int>(LDLAttrVAbsPosition));
    m_dim.AbsoluteVerticalPosition(true);
  }

  if (m_ldlBlock->Is<int>(LDLAttrHRelSize)) {
    m_dim.SetWidth(m_ldlBlock->As<int>(LDLAttrHRelSize));
    m_dim.AbsoluteHorizontalSize(false);
  } else {
    m_dim.SetWidth(m_ldlBlock->As<int>(LDLAttrHAbsSize));
    m_dim.AbsoluteHorizontalSize(true);
  }

  if (m_ldlBlock->Is<int>(LDLAttrVRelSize)) {
    m_dim.SetHeight(m_ldlBlock->As<int>(LDLAttrVRelSize));
    m_dim.AbsoluteVerticalSize(false);
  } else {
    m_dim.SetHeight(m_ldlBlock->As<int>(LDLAttrVAbsSize));
    m_dim.AbsoluteVerticalSize(true);
  }

  auto[position, width, height] = m_dim.CalculateAll();

  Region()->Pos() ;
  /*
  m_dim->CalculateAll(&m_x, &m_y, &m_width, &m_height);

  AUI_ERRCODE errcode = aui_Ldl::Associate(this, ldlBlock);
  Assert(errcode == AUI_ERRCODE_OK);
  return errcode;
  */
}

TileEngine::Region::Ptr UIRegion::Region() {
  return m_region;
}

TileEngine::Region::Ptr UIRegion::CreateChildUIRegion(const TileEngine::Position & pos,
  unsigned width, unsigned height) {
  return m_region->CreateLayer()->CreateChild(pos, width, height);
}


unsigned UIRegion::ID() const {
  return m_id;
}

const std::string &UIRegion::GetLDLBlockName() const {
  return m_ldlBlock ? m_ldlBlock->GetFullName() : std::string();
}

unsigned UIRegion::Width() const {
  return m_region ? m_region->Width() : 0;
}

unsigned UIRegion::Height() const {
  return m_region ? m_region->Height() : 0;
}

void UIRegion::MoveTo(unsigned x, unsigned y) {

}

void UIRegion::MoveBy(int dx, int dy) {

}

void UIRegion::Resize(unsigned width, unsigned height) {

}

TileEngine::Rect UIRegion::GetRect() const {
  return m_region ? m_region->GetRect() : TileEngine::Rect();
}

UIRegionPtr UIRegion::CreateChild(unsigned id,
  const TileEngine::Position &pos, unsigned width, unsigned height) {
  //UIRegion::UIRegionPtr child(std::make_shared<UIRegion>(shared_from_this(), id, pos, width, height));
  //m_children.push_back(child);
  return UIRegionPtr();
}

} // namespace ui::d3d