#include "ctp/c3.h"

#include "ui/d3d_ui/UI.h"

namespace ui::d3d {

void UI::InitializeUI(HINSTANCE hinst, int cmdshow, unsigned windowWidth, unsigned windowHeight, const std::string &ldlFile) {
  m_LDL.Load(ldlFile);

  InitializeD3d(hinst, cmdshow, windowWidth, windowHeight);

  m_scene = std::make_shared<TileEngine::Scene>(ScreenWidth(), ScreenHeight());
  GetRenderer()->SetScene(m_scene);

  m_desktopLayer = m_scene->Root()->CreateLayer(0);

  m_desktopLayer->DrawPrimitive(TileEngine::Position(),
    std::make_shared<TileEngine::ColoredRectangle>(m_desktopLayer->Width(), m_desktopLayer->Height(),
      TileEngine::MakeColor(60, 60, 60, 255)));

  m_splashLayer = m_scene->Root()->CreateLayer(0xFFFFu);
  m_mouseLayer = m_scene->Root()->CreateLayer(0xFFFFu + 1);

  m_splash = std::make_shared<Splash>(m_splashLayer, GetRenderer()->GetFontManager(), TileEngine::FontDesc("Arial", 8, TileEngine::FONTSTYLE_REGULAR));
}

UI::UI() {}

UI::~UI() {}

Splash::Ptr UI::GetSplash() {
  return m_splash;
}

void UI::HandleKeyPress(WPARAM wParam, LPARAM lParam) {}

void UI::HandleWindowsMessage(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {}

void UI::HandleMouseWheel(std::int16_t delta) {}

UICursorPtr UI::LoadUICursor(const std::string & name) {
  return m_cursorLoader.Load(name).GetUICursor();
}

void UI::AddCursorSearchPath(const std::string &path) {
  m_cursorLoader.AddPath(path);
}

UIImagePtr UI::LoadUIImage(const std::string & name) {
  return m_imageLoader.Load(name).GetUIImage();
}

void UI::AddImageSearchPath(const std::string &path) {
  m_imageLoader.AddPath(path);
}

UIIconPtr UI::LoadUIIcon(const std::string & name) {
  return m_iconLoader.Load(name).GetUIIcon();
}

void UI::AddIconSearchPath(const std::string &path) {
  m_iconLoader.AddPath(path);
}

UIPatternPtr UI::LoadUIPattern(const std::string & name) {
  return m_patternLoader.Load(name).GetUIPattern();
}

void UI::AddPatternSearchPath(const std::string &path) {
  m_patternLoader.AddPath(path);
}

UIPicturePtr UI::LoadUIPicture(const std::string & name) {
  return m_pictureLoader.Load(name).GetUIPicture();
}

void UI::AddPictureSearchPath(const std::string &path) {
  m_pictureLoader.AddPath(path);
}

unsigned UI::Width() {
  return m_scene ? m_scene->Width() : 0;
}

unsigned UI::Height() {
  return m_scene ? m_scene->Height() : 0;
}


} // namespace ui::d3d