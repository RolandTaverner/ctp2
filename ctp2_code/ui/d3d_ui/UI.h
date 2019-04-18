#pragma once

#include <string>

#include "D3dTiles/Scene.h"
#include "ui/d3d_ui/interface/D3dSplash.h"
#include "ui/d3d_ui/D3d/D3dUI.h"
#include "ui/d3d_ui/UILDL.h"

#include "ui/d3d_ui/Resource/ResourceFileManager.h"
#include "ui/d3d_ui/Resource/ResourceLoader.h"
#include "ui/d3d_ui/Resource/UICursor.h"
#include "ui/d3d_ui/Resource/UIImage.h"
#include "ui/d3d_ui/Resource/UIIcon.h"
#include "ui/d3d_ui/Resource/UIPattern.h"
#include "ui/d3d_ui/Resource/UIPicture.h"

namespace ui::d3d {

class UI : public ui::d3d::Direct3d::D3dUI {
public:
  UI();
  virtual ~UI();
  UI(const UI&) = delete;
  UI &operator=(const UI &) = delete;

  Splash::Ptr GetSplash();
  
  unsigned Width();
  unsigned Height();

  void HandleKeyPress(WPARAM wParam, LPARAM lParam) override;
  void HandleWindowsMessage(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) override;
  void HandleMouseWheel(std::int16_t delta) override;
  
  UICursorPtr LoadUICursor(const std::string &name);
  UIImagePtr LoadUIImage(const std::string &name);
  UIIconPtr LoadUIIcon(const std::string &name);
  UIPatternPtr LoadUIPattern(const std::string &name);
  UIPicturePtr LoadUIPicture(const std::string &name);
  
protected:
  void InitializeUI(HINSTANCE hinst, int cmdshow, unsigned windowWidth, unsigned windowHeight, const std::string & ldlFile);
  ResourceFileManagerPtr GetFileManager();

private:
  TileEngine::Scene::Ptr m_scene;
  TileEngine::Region::Ptr m_desktopLayer;
  TileEngine::Region::Ptr m_mouseLayer;
  TileEngine::Region::Ptr m_splashLayer;
  Splash::Ptr m_splash;

  LDL m_LDL;

  ResourceFileManagerPtr m_fileManager;
  ResourceLoader<UICursorResource> m_cursorLoader;
  ResourceLoader<UIImageResource> m_imageLoader;
  ResourceLoader<UIIconResource> m_iconLoader;
  ResourceLoader<UIPatternResource> m_patternLoader;
  ResourceLoader<UIPictureResource> m_pictureLoader;
};

} // namespace ui::d3d