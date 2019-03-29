#pragma once

#include <memory>

#include "D3dTiles/Region.h"

namespace ui::d3d {

class Splash {
public:
  typedef std::shared_ptr<Splash> Ptr;

  explicit Splash(TileEngine::Region::Ptr region);

  void AddText(MBCHAR const *text);
  void AddTextNL(MBCHAR const *text);
  void AddHilitedTextNL(MBCHAR const *text);

private:
  TileEngine::Region::Ptr m_layer;
  TileEngine::Region::Ptr m_textArea;
};

} // namespace ui::d3d