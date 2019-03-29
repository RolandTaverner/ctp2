#pragma once

#include <cstdint>
#include <memory>

#include <windows.h>

#include "D3dTiles/Scene.h"
#include "D3dTiles/D3d/Renderer.h"

#include "ui/d3d_ui/interface/D3dSplash.h"

namespace ui::d3d {

extern LPCSTR	gszMainWindowClass;
extern LPCSTR	gszMainWindowName;

class D3dUI : public std::enable_shared_from_this<D3dUI> {
public:
  typedef std::shared_ptr<D3dUI> D3dUIPtr;

  D3dUI();
  D3dUI(const D3dUI&) = delete;
  D3dUI(D3dUI&&) = delete;
  D3dUI& operator=(const D3dUI&) = delete;
  D3dUI& operator=(D3dUI&&) = delete;
  virtual ~D3dUI();

  void Initialize(HINSTANCE hinst, int cmdshow, unsigned windowWidth, unsigned windowHeight);
  unsigned Width();
  unsigned Height();
  HWND Wnd();
  HINSTANCE Hinst();
  void HandleKeyPress(WPARAM wParam, LPARAM lParam);
  void HandleWindowsMessage(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
  void HandleMouseWheel(std::int16_t delta);
  void Render();
  void Destroy();

  static D3dUIPtr Self();
  static void Free();
  Splash::Ptr GetSplash();

private:
  void InitWindow(int cmdshow, unsigned windowWidth, unsigned windowHeight);

private:
  TileEngine::D3d::RendererPtr m_renderer;
  TileEngine::Scene::Ptr m_scene;
  TileEngine::Region::Ptr m_desktopLayer;
  TileEngine::Region::Ptr m_mouseLayer;
  TileEngine::Region::Ptr m_splashLayer;

  Splash::Ptr m_splash;

  HWND m_hWnd;
  HINSTANCE m_hinst;

  static D3dUIPtr m_self;
};

} // namespace ui::d3d
