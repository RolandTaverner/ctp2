#include "ctp/c3.h"

#include "ui/d3d_ui/UI.h"

namespace ui::d3d {

void UI::InitializeUI(HINSTANCE hinst, int cmdshow, unsigned windowWidth, unsigned windowHeight, const std::string &ldlFile) {
  auto fi = GetFileManager()->FindFile(ldlFile, PATH_LAYOUT);
  if (!fi) {
    throw std::runtime_error("Can't find file " + ldlFile);
  }

  m_LDL.Load(fi.GetFullPath());

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

ResourceFileManagerPtr UI::GetFileManager() {
  return  m_fileManager;
}

UI::UI() :
  m_fileManager(std::make_shared<ResourceFileManager>()), 
  m_cursorLoader(m_fileManager, ResourcePathContentTags{ PATH_CURSORS }),
  m_imageLoader(m_fileManager, ResourcePathContentTags{ PATH_PICTURES, PATH_PATTERNS, PATH_ICONS, PATH_CURSORS }),
  m_iconLoader(m_fileManager, ResourcePathContentTags{ PATH_ICONS }),
  m_patternLoader(m_fileManager, ResourcePathContentTags{ PATH_PATTERNS, PATH_PICTURES }),
  m_pictureLoader(m_fileManager, ResourcePathContentTags{ PATH_PICTURES }) {}

UI::~UI() {}

Splash::Ptr UI::GetSplash() {
  return m_splash;
}

void UI::HandleKeyPress(WPARAM wParam, LPARAM lParam) {}

void UI::HandleWindowsMessage(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {}

void UI::HandleMouseWheel(std::int16_t delta) {}

UICursorPtr UI::LoadUICursor(const std::string &name) {
  return m_cursorLoader.Load(name, PATH_CURSORS).GetUICursor();
}

UIImagePtr UI::LoadUIImage(const std::string &name) {
  return m_imageLoader.Load(name, PATH_PICTURES).GetUIImage();
}

UIIconPtr UI::LoadUIIcon(const std::string &name) {
  return m_iconLoader.Load(name, PATH_DIRECT).GetUIIcon();
}

UIPatternPtr UI::LoadUIPattern(const std::string &name) {
  return m_patternLoader.Load(name, PATH_DIRECT).GetUIPattern();
}

UIPicturePtr UI::LoadUIPicture(const std::string &name) {
  return m_pictureLoader.Load(name, PATH_DIRECT).GetUIPicture();
}

unsigned UI::Width() {
  return m_scene ? m_scene->Width() : 0;
}

unsigned UI::Height() {
  return m_scene ? m_scene->Height() : 0;
}


} // namespace ui::d3d