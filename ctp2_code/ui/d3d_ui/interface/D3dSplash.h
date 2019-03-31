#pragma once

#include <memory>
#include <deque>
#include <string>

#include "D3dTiles/FontManager.h"
#include "D3dTiles/Region.h"

namespace ui::d3d {

class Splash {
public:
  typedef std::shared_ptr<Splash> Ptr;

  explicit Splash(TileEngine::Region::Ptr region, TileEngine::FontManager::Ptr fontManager, TileEngine::FontDesc fontDesc);

  void AddTextNL(const std::string &str);
  void AddHilitedTextNL(const std::string &str);

private:
  TileEngine::Region::Ptr m_layer;
  TileEngine::FontManager::Ptr m_fontManager;
  TileEngine::FontDesc m_fontDesc;
  TileEngine::Region::Ptr m_textArea;

  unsigned m_currentY;
  std::deque<std::string> m_messages;
};

} // namespace ui::d3d