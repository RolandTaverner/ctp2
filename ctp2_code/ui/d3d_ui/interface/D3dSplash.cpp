#include "ctp/c3.h"

#include <iomanip>
#include <chrono>
#include <sstream>

#include "D3dTiles/Primitives/ColoredRectangle.h"

#include "ui/d3d_ui/interface/D3dSplash.h"

static const unsigned s_splashMargin = 10;

namespace te = TileEngine;

namespace ui::d3d {

Splash::Splash(te::Region::Ptr region,
  TileEngine::FontManager::Ptr fontManager,
  TileEngine::FontDesc fontDesc) :
  m_layer(region), m_fontManager(fontManager), m_fontDesc(fontDesc), m_currentY(0) {
  m_textArea = m_layer->CreateChild(
    te::Position(s_splashMargin, s_splashMargin),
    m_layer->Width() - 2 * s_splashMargin, m_layer->Height() - 2 * s_splashMargin);
}

void Splash::AddTextNL(const std::string &str) {
  std::time_t timePoint = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm tm = *std::localtime(&timePoint);

  std::ostringstream s;
  s << std::put_time(std::localtime(&timePoint), "%H:%M:%S") << " " << str;
  const std::string newMsg = s.str();

  TileEngine::Text::Ptr text(std::make_shared<TileEngine::Text>(
    m_fontManager, m_fontDesc, newMsg, TileEngine::MakeColor(0xFFu, 0xFFu, 0xFFu, 0xFFu)));

  const unsigned textHeight = text->Height();
  
  std::deque<std::string> messagesNew;
  messagesNew.push_front(newMsg);
  unsigned newHeight = textHeight;

  if (m_currentY + textHeight > m_textArea->Height()) {
    for (auto oldMsg : m_messages) {
      TileEngine::Text::Ptr oldMsgText(std::make_shared<TileEngine::Text>(
        m_fontManager, m_fontDesc, oldMsg, TileEngine::MakeColor(0xFFu, 0xFFu, 0xFFu, 0xFFu)));
      const unsigned oldMsgTextHeight = oldMsgText->Height();
      if (newHeight + oldMsgTextHeight <= m_textArea->Height()) {
        messagesNew.push_front(oldMsg);
        newHeight += oldMsgTextHeight;
      } else {
        break;
      }
    }
    m_messages.swap(messagesNew);
    m_layer->ClearPrimitives();
    m_currentY = 0;

    for (auto oldMsg : m_messages) {
      TileEngine::Text::Ptr oldMsgText(std::make_shared<TileEngine::Text>(
        m_fontManager, m_fontDesc, oldMsg, TileEngine::MakeColor(0xFFu, 0xFFu, 0xFFu, 0xFFu)));
      const unsigned oldMsgTextHeight = oldMsgText->Height();
      m_layer->DrawPrimitive(TileEngine::Position(0, m_currentY), oldMsgText);
      m_currentY += textHeight;
    }
  } else {
    m_layer->DrawPrimitive(TileEngine::Position(0, m_currentY), text);
    m_currentY += textHeight;
  }
}

void Splash::AddHilitedTextNL(const std::string &str) {}

} // namespace ui::d3d 