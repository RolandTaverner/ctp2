#pragma once

#include <cstdint>
#include <memory>

#include <windows.h>

#include "D3dTiles/D3d/Renderer.h"

namespace ui::d3d::Direct3d {

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

  HWND Wnd();
  HINSTANCE Hinst();
  unsigned ScreenWidth() const;
  unsigned ScreenHeight() const;
  void Render();
  void Destroy();

  static D3dUIPtr GetD3dUI();

  virtual void HandleKeyPress(WPARAM wParam, LPARAM lParam) = 0;
  virtual void HandleWindowsMessage(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) = 0;
  virtual void HandleMouseWheel(std::int16_t delta) = 0;

protected:
  void InitializeD3d(HINSTANCE hinst, int cmdshow, unsigned windowWidth, unsigned windowHeight);

  TileEngine::D3d::RendererPtr GetRenderer();
  TileEngine::FontManager::Ptr GetFontManager();

  static D3dUIPtr m_self;
  static void Free();

private:
  void InitWindow(int cmdshow, unsigned windowWidth, unsigned windowHeight);

private:
  unsigned m_screenWidth, m_screenHeight;
  TileEngine::D3d::RendererPtr m_renderer;

  HWND m_hWnd;
  HINSTANCE m_hinst;
};

} // namespace ui::d3d::Direct3d
