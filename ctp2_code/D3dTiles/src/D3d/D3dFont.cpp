#include "stdafx.h"

#include <codecvt>
#include <stdexcept>

#include "D3dTiles/D3d/D3dFont.h"

namespace TileEngine::D3d {

D3dFont::D3dFont(SpriteFontPtr spriteFont, const std::string &fontFace, FontStyle style) :
  TileEngine::Font(fontFace, style), m_spriteFont(std::move(spriteFont)) {}

D3dFont::~D3dFont() {}

Rect D3dFont::GetTextRect(const std::string &text, const Position &pos) const {
  const DirectX::XMVECTOR res = m_spriteFont->MeasureString(text.c_str());
  const Position wh(DirectX::XMVectorGetX(res), DirectX::XMVectorGetY(res));

  const Position &minPoint(pos);
  Position maxPoint(pos);
  boost::geometry::add_point(maxPoint, wh);

  return Rect(minPoint, maxPoint);
}

Rect D3dFont::GetTextRect(const std::string &text) const {
  return GetTextRect(text, Position(0, 0));
}

unsigned D3dFont::GetTextHeight(const std::string &text) const {
  const DirectX::XMVECTOR res = m_spriteFont->MeasureString(text.c_str());
  return DirectX::XMVectorGetY(res);
}

unsigned D3dFont::GetTextWidth(const std::string &text) const {
  const DirectX::XMVECTOR res = m_spriteFont->MeasureString(text.c_str());
  return DirectX::XMVectorGetX(res);
}

unsigned D3dFont::GetFontHeight() {
  // TODO
  return 0;
}

const DirectX::SpriteFont & D3dFont::GetSpriteFont() const {
  if (!m_spriteFont) {
    throw std::runtime_error("Empty SpriteFont");
  }

  return *m_spriteFont.get();
}

} // namespace TileEngine::D3d