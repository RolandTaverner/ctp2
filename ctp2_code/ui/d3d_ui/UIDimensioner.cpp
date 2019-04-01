#include "ctp/c3.h"

#include "ui/d3d_ui/UIRegion.h"

#include "ui/d3d_ui/UIDimensioner.h"

namespace ui::d3d {

UIDimensioner::UIDimensioner() : UIDimensioner(UIRegionPtr()) {}

UIDimensioner::UIDimensioner(UIRegionPtr owner) :
  m_owner(owner), m_position(0, 0), m_width(0), m_height(0),
  m_hAnchorType(AUI_DIMENSION_HANCHOR_LEFT),
  m_vAnchorType(AUI_DIMENSION_VANCHOR_TOP),
  m_hPosType(AUI_DIMENSION_HPOSITION_ABSOLUTE),
  m_vPosType(AUI_DIMENSION_VPOSITION_ABSOLUTE),
  m_hSizeType(AUI_DIMENSION_HSIZE_ABSOLUTE),
  m_vSizeType(AUI_DIMENSION_VSIZE_ABSOLUTE) {}

UIDimensioner::~UIDimensioner() {}

void UIDimensioner::SetOwner(UIRegionPtr owner) {
  m_owner = owner;
}

UIRegionPtr UIDimensioner::GetOwner(void) const {
  return m_owner.lock();
}

AUI_DIMENSION_HANCHOR UIDimensioner::GetHorizontalAnchorType(void) const {
  return AUI_DIMENSION_HANCHOR(m_hAnchorType);
}

AUI_DIMENSION_VANCHOR UIDimensioner::GetVerticalAnchorType(void) const {
  return AUI_DIMENSION_VANCHOR(m_vAnchorType);
}

AUI_DIMENSION_HPOSITION UIDimensioner::GetHorizontalPositionType(void) const {
  return AUI_DIMENSION_HPOSITION(m_hPosType);
}

AUI_DIMENSION_VPOSITION UIDimensioner::GetVerticalPositionType(void) const {
  return AUI_DIMENSION_VPOSITION(m_vPosType);
}

AUI_DIMENSION_HSIZE UIDimensioner::GetHorizontalSizeType(void) const {
  return AUI_DIMENSION_HSIZE(m_hSizeType);
}

AUI_DIMENSION_VSIZE UIDimensioner::GetVerticalSizeType(void) const {
  return AUI_DIMENSION_VSIZE(m_vSizeType);
}

const TileEngine::Position &UIDimensioner::GetPosition() const {
  return m_position;
}

void UIDimensioner::SetPosition(const TileEngine::Position &position) {
  m_position = position;
}

unsigned UIDimensioner::GetWidth() const {
  return m_width;
}

void UIDimensioner::SetWidth(unsigned width) {
  m_width = width;
}

unsigned UIDimensioner::GetHeight() const {
  return m_height;
}

void UIDimensioner::SetHeight(unsigned height) {
  m_height = height;
}

void UIDimensioner::AnchorLeft(void) {
  m_hAnchorType = AUI_DIMENSION_HANCHOR_LEFT;
}

void UIDimensioner::AnchorHorizontalCenter(void) {
  m_hAnchorType = AUI_DIMENSION_HANCHOR_CENTER;
}

void UIDimensioner::AnchorRight(void) {
  m_hAnchorType = AUI_DIMENSION_HANCHOR_RIGHT;
}

void UIDimensioner::AnchorTop(void) {
  m_vAnchorType = AUI_DIMENSION_VANCHOR_TOP;
}

void UIDimensioner::AnchorVerticalCenter(void) {
  m_vAnchorType = AUI_DIMENSION_VANCHOR_CENTER;
}

void UIDimensioner::AnchorBottom(void) {
  m_vAnchorType = AUI_DIMENSION_VANCHOR_BOTTOM;
}

void UIDimensioner::AbsoluteHorizontalPosition(bool absolute) {
  m_hPosType = absolute ? AUI_DIMENSION_HPOSITION_ABSOLUTE : AUI_DIMENSION_HPOSITION_RELATIVE;
}

void UIDimensioner::AbsoluteVerticalPosition(bool absolute) {
  m_vPosType = absolute ? AUI_DIMENSION_VPOSITION_ABSOLUTE : AUI_DIMENSION_VPOSITION_RELATIVE;
}

void UIDimensioner::AbsoluteHorizontalSize(bool absolute) {
  m_hSizeType = absolute ? AUI_DIMENSION_HSIZE_ABSOLUTE : AUI_DIMENSION_HSIZE_RELATIVE;
}

void UIDimensioner::AbsoluteVerticalSize(bool absolute) {
  m_vSizeType = absolute ? AUI_DIMENSION_VSIZE_ABSOLUTE : AUI_DIMENSION_VSIZE_RELATIVE;
}

int UIDimensioner::CalculateX(void) {
  UIRegionPtr owner = m_owner.lock();
  if (!owner) {
    return m_position.get<0>();
  }
  int result = m_position.get<0>();

  switch (m_hAnchorType) {
    case AUI_DIMENSION_HANCHOR_LEFT:
    {
      if (m_hPosType == AUI_DIMENSION_HPOSITION_RELATIVE) {
        result = int((double)owner->Width() * m_position.get<0>() / 100.0 + 0.5);
      }
      break;
    }
    case AUI_DIMENSION_HANCHOR_CENTER:
    {
      result = (owner->Width() - CalculateWidth()) / 2;
      break;
    }
    case AUI_DIMENSION_HANCHOR_RIGHT:
    {
      int x = m_position.get<0>();
      if (m_hPosType == AUI_DIMENSION_HPOSITION_RELATIVE) {
        x *= int((double)owner->Width() / 100.0 + 0.5);
      }
      result = owner->Width() - CalculateWidth() - x;
      break;
    }
    default:
    {
      Assert(FALSE);
    }
  }

  return result;
}

int UIDimensioner::CalculateY(void) {
  UIRegionPtr owner = m_owner.lock();
  if (!owner) {
    return m_position.get<1>();
  }
  int result = m_position.get<1>();

  switch (m_vAnchorType) {
    case AUI_DIMENSION_VANCHOR_TOP:
    {
      if (m_vPosType == AUI_DIMENSION_VPOSITION_RELATIVE) {
        result = int((double)owner->Height() * m_position.get<1>() / 100.0 + 0.5);
      }
      break;
    }
    case AUI_DIMENSION_VANCHOR_CENTER:
    {
      result = (owner->Height() - CalculateHeight()) / 2;
      break;
    }
    case AUI_DIMENSION_VANCHOR_BOTTOM:
    {
      int y = m_position.get<1>();
      if (m_vPosType == AUI_DIMENSION_VPOSITION_RELATIVE) {
        y *= int((double)owner->Height() / 100.0 + 0.5);
      }
      result = owner->Height() - CalculateHeight() - y;
      break;
    }
    default:
    {
      Assert(FALSE);
    }
  }

  return result;
}

unsigned UIDimensioner::CalculateWidth(void) {
  UIRegionPtr owner = m_owner.lock();
  if (!owner) {
    return m_width;
  }

  if (m_hSizeType == AUI_DIMENSION_HSIZE_ABSOLUTE) {
    return m_width;
  } else if (m_hSizeType == AUI_DIMENSION_HSIZE_RELATIVE) {
    return unsigned((double)owner->Width() * m_width / 100.0 + 0.5);
  }
  Assert(FALSE);
  return m_width;
}

unsigned UIDimensioner::CalculateHeight(void) {
  UIRegionPtr owner = m_owner.lock();
  if (!owner) {
    return m_height;
  }

  if (m_vSizeType == AUI_DIMENSION_VSIZE_ABSOLUTE) {
    return m_height;
  } else if (m_vSizeType == AUI_DIMENSION_VSIZE_RELATIVE) {
    return unsigned((double)owner->Height() * m_height / 100.0 + 0.5);
  }
  Assert(FALSE);
  return m_height;
}

void UIDimensioner::SetCalcHorizontalPosition(int x) {
  UIRegionPtr owner = m_owner.lock();
  if (!owner) {
    m_position.set<0>(x);
    return;
  }

  if (m_hAnchorType == AUI_DIMENSION_HANCHOR_LEFT) {
    if (m_hPosType == AUI_DIMENSION_HPOSITION_RELATIVE) {
      m_position.set<0>(int((double)x * 100.0 / owner->Width() + 0.5));
    } else {
      m_position.set<0>(x);
    }
  } else if (m_hAnchorType == AUI_DIMENSION_HANCHOR_CENTER) {
    m_position.set<0>(x);
  } else if (m_hAnchorType == AUI_DIMENSION_HANCHOR_RIGHT) {
    m_position.set<0>(owner->Width() - CalculateWidth() - x);

    if (m_hPosType == AUI_DIMENSION_HPOSITION_RELATIVE)
      m_position.set<0>(sint32((double)m_position.get<0>() * 100.0 / owner->Width() + 0.5));
  } else {
    Assert(FALSE);
  }
}

void UIDimensioner::SetCalcVerticalPosition(int y) {
  UIRegionPtr owner = m_owner.lock();
  if (!owner) {
    m_position.set<1>(y);
    return;
  }

  if (m_vAnchorType == AUI_DIMENSION_VANCHOR_TOP) {
    if (m_vPosType == AUI_DIMENSION_VPOSITION_RELATIVE) {
      m_position.set<1>(int((double)y * 100.0 / owner->Height() + 0.5));
    } else {
      m_position.set<1>(y);
    }
  } else if (m_vAnchorType == AUI_DIMENSION_VANCHOR_CENTER) {
    m_position.set<1>(y);
  } else if (m_vAnchorType == AUI_DIMENSION_VANCHOR_BOTTOM) {
    m_position.set<1>(owner->Height() - CalculateHeight() - y);

    if (m_vPosType == AUI_DIMENSION_VPOSITION_RELATIVE) {
      m_position.set<1>(int((double)m_position.get<1>() * 100.0 / owner->Height() + 0.5));
    }
  } else {
    Assert(FALSE);
  }
}

void UIDimensioner::SetCalcWidth(unsigned width) {
  UIRegionPtr owner = m_owner.lock();
  if (!owner) {
    m_width = width;
    return;
  }

  if (m_hSizeType == AUI_DIMENSION_HSIZE_ABSOLUTE) {
    m_width = width;
  } else if (m_hSizeType == AUI_DIMENSION_HSIZE_RELATIVE) {
    m_width = unsigned((double)width * 100.0 / owner->Width() + 0.5);
  } else {
    Assert(FALSE);
  }
}

void UIDimensioner::SetCalcHeight(unsigned height) {
  UIRegionPtr owner = m_owner.lock();
  if (!owner) {
    m_height = height;
    return;
  };

  if (m_vSizeType == AUI_DIMENSION_VSIZE_ABSOLUTE) {
    m_height = height;
  } else if (m_vSizeType == AUI_DIMENSION_VSIZE_RELATIVE) {
    m_height = unsigned((double)height * 100.0 / owner->Height() + 0.5);
  } else {
    Assert(FALSE);
  }
}

} // namespace ui::d3d