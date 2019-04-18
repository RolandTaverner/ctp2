#pragma once

#include <cstdint>
#include <tuple>

#include "D3dTiles/Geometry.h"

#include "ui/d3d_ui/UIRegionFwd.h"

namespace ui::d3d {

enum AUI_DIMENSION_HANCHOR : std::uint8_t {
  AUI_DIMENSION_HANCHOR_LEFT = 0x00,
  AUI_DIMENSION_HANCHOR_CENTER = 0x01,
  AUI_DIMENSION_HANCHOR_RIGHT = 0x02
};

enum AUI_DIMENSION_VANCHOR : std::uint8_t {
  AUI_DIMENSION_VANCHOR_TOP = 0x00,
  AUI_DIMENSION_VANCHOR_CENTER = 0x01,
  AUI_DIMENSION_VANCHOR_BOTTOM = 0x02
};

enum AUI_DIMENSION_HPOSITION : std::uint8_t {
  AUI_DIMENSION_HPOSITION_ABSOLUTE = 0x00,
  AUI_DIMENSION_HPOSITION_RELATIVE = 0x01

};

enum AUI_DIMENSION_VPOSITION : std::uint8_t {
  AUI_DIMENSION_VPOSITION_ABSOLUTE = 0x00,
  AUI_DIMENSION_VPOSITION_RELATIVE = 0x01
};

enum AUI_DIMENSION_HSIZE : std::uint8_t {
  AUI_DIMENSION_HSIZE_ABSOLUTE = 0x00,
  AUI_DIMENSION_HSIZE_RELATIVE = 0x01
};

enum AUI_DIMENSION_VSIZE : std::uint8_t {
  AUI_DIMENSION_VSIZE_ABSOLUTE = 0x00,
  AUI_DIMENSION_VSIZE_RELATIVE = 0x01
};

class UIDimensioner {
public:
  UIDimensioner();
  explicit UIDimensioner(UIRegionPtr owner);
  virtual ~UIDimensioner();

  void SetOwner(UIRegionPtr owner);
  UIRegionPtr GetOwner(void) const;

  AUI_DIMENSION_HANCHOR GetHorizontalAnchorType(void) const;
  AUI_DIMENSION_VANCHOR GetVerticalAnchorType(void) const;
  AUI_DIMENSION_HPOSITION GetHorizontalPositionType(void) const;
  AUI_DIMENSION_VPOSITION GetVerticalPositionType(void) const;
  AUI_DIMENSION_HSIZE GetHorizontalSizeType(void) const;
  AUI_DIMENSION_VSIZE GetVerticalSizeType(void) const;

  // Direct access
  const TileEngine::Position &GetPosition() const;
  void SetPosition(const TileEngine::Position &position);
  void SetHorizontalPosition(int x);
  void SetVerticalPosition(int y);
  unsigned GetWidth() const;
  void SetWidth(unsigned width);
  unsigned GetHeight() const;
  void SetHeight(unsigned height);

  void AnchorLeft(void);
  void AnchorHorizontalCenter(void);
  void AnchorRight(void);
  void AnchorTop(void);
  void AnchorVerticalCenter(void);
  void AnchorBottom(void);

  void AbsoluteHorizontalPosition(bool absolute);
  void AbsoluteVerticalPosition(bool absolute);
  void AbsoluteHorizontalSize(bool absolute);
  void AbsoluteVerticalSize(bool absolute);

  void SetCalcHorizontalPosition(int x);
  void SetCalcVerticalPosition(int y);
  void SetCalcWidth(unsigned width);
  void SetCalcHeight(unsigned height);

  int CalculateX(void) const;
  int CalculateY(void) const;
  unsigned CalculateWidth(void) const;
  unsigned CalculateHeight(void) const;

  std::tuple<TileEngine::Position, unsigned, unsigned> CalculateAll() const;

private:
  UIRegionWPtr m_owner;
  TileEngine::Position m_position;
  std::uint32_t m_width, m_height;
  // Flags
  std::uint32_t m_hAnchorType : 2;
  std::uint32_t m_vAnchorType : 2;
  std::uint32_t m_hPosType    : 1;
  std::uint32_t m_vPosType    : 1;
  std::uint32_t m_hSizeType   : 1;
  std::uint32_t m_vSizeType   : 1;
};

} // namespace ui::d3d