#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "D3dTiles/Color.h"
#include "D3dTiles/Font.h"
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

  Text(Font::Ptr font, const std::string &text,
    Color textColor, Color dropColor, const Position &center, const Rect &bounds, TextOpts options);
  Text(Font::Ptr font, const std::string &text, Color textColor); // Text
  Text(Font::Ptr font, const std::string &text, Color textColor, Color dropColor); // Text with shadow
  Text(Font::Ptr font, const std::string &text, Color textColor, const Position &center); // Centered text
  Text(Font::Ptr font, const std::string &text, Color textColor, const Rect &bounds); // Bounded

  virtual ~Text();

  template<typename ...Args>
  static Ptr New(Args...) {
    return std::make_shared<Text>(Args...);
  }

private:
  Font::Ptr m_font;
  std::string m_text;
  Color m_textColor;
  Color m_dropColor;
  Position m_center;
  Rect m_bounds;
  TextOpts m_options;
};

} // namespace TileEngine
