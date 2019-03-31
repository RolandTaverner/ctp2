#pragma once

#include <DirectXMath.h>

#include "DirectXTK/SpriteFont.h"

#include "D3dTiles/Font.h"

namespace TileEngine::D3d {

typedef std::unique_ptr<DirectX::SpriteFont> SpriteFontPtr;

class D3dFont : public TileEngine::Font {
public:
  typedef std::shared_ptr<D3dFont> Ptr;

  D3dFont(SpriteFontPtr spriteFont, const std::string &fontFace, unsigned fontSize, FontStyle style);
  virtual ~D3dFont();

  Rect GetTextRect(const std::string &text) const override;
  Rect GetTextRect(const std::string &text, const Position &pos) const override;
  unsigned GetTextHeight(const std::string &text) const override;
  unsigned GetTextWidth(const std::string &text) const override;
  unsigned GetFontHeight() override;

  const DirectX::SpriteFont &GetSpriteFont() const;

private:
  SpriteFontPtr m_spriteFont;
};

} // namespace 