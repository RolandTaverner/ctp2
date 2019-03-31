#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "D3dTiles/Color.h"
#include "D3dTiles/Font.h"
#include "D3dTiles/FontManager.h"
#include "D3dTiles/Geometry.h"
#include "D3dTiles/Primitives/Primitive.h"

namespace TileEngine {

enum TextOpts : std::uint32_t {
  SIMPLETEXT = 0x0,
  DROPTEXT = 0x00000001,
  CENTERTEXT = 0x00000002,
  BOUNDTEXT = 0x00000004,
};

class Text : public Primitive {
public:
  typedef std::shared_ptr<Text> Ptr;

  Text(FontManager::Ptr fontManager, FontDesc font, const std::string &text, Color textColor,
    Color dropColor, const Position &center, const Rect &bounds, TextOpts options, float scale);

  Text(FontManager::Ptr fontManager, FontDesc font, const std::string &text, Color textColor, float scale = 1.0f); // Text

  Text(FontManager::Ptr fontManager, FontDesc font, const std::string &text, Color textColor,
    Color dropColor, float scale = 1.0f); // Text with shadow

  Text(FontManager::Ptr fontManager, FontDesc font, const std::string &text, Color textColor,
    const Position &center, float scale = 1.0f); // Centered text

  Text(FontManager::Ptr fontManager, FontDesc font, const std::string &text, Color textColor,
    const Rect &bounds, float scale = 1.0f); // Bounded

  const std::string GetFontFace() const;
  FontStyle GetFontStyle() const;
  const std::string GetText() const;
  Color GetTextColor() const;
  float GetScale() const;

  // Primitive
  unsigned Width() const override;
  unsigned Height() const override;

  virtual ~Text();

  template<typename ...Args>
  static Ptr New(Args...) {
    return std::make_shared<Text>(Args...);
  }

private:
  FontManager::Ptr m_fontManager;
  FontDesc m_font;
  std::string m_text;
  Color m_textColor;
  Color m_dropColor;
  Position m_center;
  Rect m_bounds;
  TextOpts m_options;
  float m_scale;
};

} // namespace TileEngine
