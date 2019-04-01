#include "ctp/c3.h"

#include <stdexcept>

#include "ui/aui_common/aui_ldl.h"
#include "ui/ldl/ldl_attr.hpp"
#include "ui/ldl/ldl_data.hpp"

#include "ui/d3d_ui/UIRegion.h"

namespace ui::d3d {

UIRegion::UIRegion(unsigned id,
  const TileEngine::Position &pos, unsigned width, unsigned height) :
  m_id(id),
  m_region(std::make_shared<TileEngine::Region>(TileEngine::Region::Ptr(), id, pos, width, height, true, true)) {}

UIRegion::UIRegion(unsigned id, const std::string &ldlBlockName) :
  m_id(id), m_ldlBlockName(ldlBlockName) {
  InitFromLDL();
}

UIRegion::~UIRegion() {}

void UIRegion::InitFromLDL() {
  /*
  ldl_datablock *block = aui_Ldl::FindDataBlock(m_ldlBlockName.c_str());
  Assert(block != NULL);
  if (!block) {
    throw std::runtime_error("Ldl block " + m_ldlBlockName + " not found");
  }

  aui_Region *parent = NULL;

  if (block->GetAttributeType(k_AUI_LDL_PARENT) == ATTRIBUTE_TYPE_STRING) {
    parent = (aui_Region *)aui_Ldl::GetObject(block->GetString(k_AUI_LDL_PARENT));
    Assert(parent != NULL);
  }

  if (!parent) {
    MBCHAR const * lastDot = strrchr(ldlBlock, '.');
    if (lastDot) {
      MBCHAR tempStr[k_AUI_LDL_MAXBLOCK];

      strncpy(tempStr, ldlBlock, (lastDot - ldlBlock));
      tempStr[lastDot - ldlBlock] = '\0';

      parent = (aui_Region *)aui_Ldl::GetObject(tempStr);
    } else {
      parent = g_ui;
    }
  }

  m_dim->SetParent(parent);

  if (block->GetAttributeType(k_AUI_REGION_LDL_BLINDNESS))
    m_blind = block->GetBool(k_AUI_REGION_LDL_BLINDNESS);

  if (MBCHAR * horizontalAnchor = block->GetString(k_AUI_LDL_HANCHOR)) {
    if (stricmp(horizontalAnchor, "right") == 0) {
      m_dim->AnchorRight();
    } else if (stricmp(horizontalAnchor, "center") == 0) {
      m_dim->AnchorHorizontalCenter();
    } else {
      m_dim->AnchorLeft();
    }
  } else
    m_dim->AnchorLeft();

  if (MBCHAR * verticalAnchor = block->GetString(k_AUI_LDL_VANCHOR)) {
    if (stricmp(verticalAnchor, "bottom") == 0) {
      m_dim->AnchorBottom();
    } else if (stricmp(verticalAnchor, "center") == 0) {
      m_dim->AnchorVerticalCenter();
    } else {
      m_dim->AnchorTop();
    }
  } else
    m_dim->AnchorTop();

  if (block->GetAttributeType(k_AUI_LDL_HRELPOSITION) == ATTRIBUTE_TYPE_INT) {
    m_dim->HorizontalPositionData() = block->GetInt(k_AUI_LDL_HRELPOSITION);
    m_dim->AbsoluteHorizontalPosition(FALSE);
  } else {
    m_dim->HorizontalPositionData() = block->GetInt(k_AUI_LDL_HABSPOSITION);
    m_dim->AbsoluteHorizontalPosition(TRUE);
  }

  if (block->GetAttributeType(k_AUI_LDL_VRELPOSITION) == ATTRIBUTE_TYPE_INT) {
    m_dim->VerticalPositionData() = block->GetInt(k_AUI_LDL_VRELPOSITION);
    m_dim->AbsoluteVerticalPosition(FALSE);
  } else {
    m_dim->VerticalPositionData() = block->GetInt(k_AUI_LDL_VABSPOSITION);
    m_dim->AbsoluteVerticalPosition(TRUE);
  }

  if (block->GetAttributeType(k_AUI_LDL_HRELSIZE) == ATTRIBUTE_TYPE_INT) {
    m_dim->HorizontalSizeData() = block->GetInt(k_AUI_LDL_HRELSIZE);
    m_dim->AbsoluteHorizontalSize(FALSE);
  } else {
    m_dim->HorizontalSizeData() = block->GetInt(k_AUI_LDL_HABSSIZE);
    m_dim->AbsoluteHorizontalSize(TRUE);
  }

  if (block->GetAttributeType(k_AUI_LDL_VRELSIZE) == ATTRIBUTE_TYPE_INT) {
    m_dim->VerticalSizeData() = block->GetInt(k_AUI_LDL_VRELSIZE);
    m_dim->AbsoluteVerticalSize(FALSE);
  } else {
    m_dim->VerticalSizeData() = block->GetInt(k_AUI_LDL_VABSSIZE);
    m_dim->AbsoluteVerticalSize(TRUE);
  }

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

UIRegion::UIRegionPtr UIRegion::CreateChild(unsigned id,
  const TileEngine::Position &pos, unsigned width, unsigned height) {
  //UIRegion::UIRegionPtr child(std::make_shared<UIRegion>(shared_from_this(), id, pos, width, height));
  //m_children.push_back(child);
  return UIRegion::UIRegionPtr();
}

} // namespace ui::d3d