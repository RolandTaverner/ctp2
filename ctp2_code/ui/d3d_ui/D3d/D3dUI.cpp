#include "ctp/c3.h"

#include <filesystem>
#include <stdexcept>

#include <comutil.h>

#include "ui/d3d_ui/D3d/D3dUI.h"

#define k_MSWHEEL_ROLLMSG		0xC7AF

namespace ui::d3d::Direct3d {

LPCSTR gszMainWindowClass = "CTP II";
LPCSTR gszMainWindowName = "CTP II";

D3dUI::D3dUIPtr D3dUI::m_self;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

D3dUI::D3dUI() :
  m_screenWidth(0), m_screenHeight(0), m_hWnd(0), m_hinst(0) {}

D3dUI::~D3dUI() {}

void D3dUI::InitializeD3d(HINSTANCE hinst, int cmdshow,
  unsigned windowWidth, unsigned windowHeight) {
  m_renderer = std::make_shared<TileEngine::D3d::Renderer>();
  m_hinst = hinst;

  InitWindow(cmdshow, windowWidth, windowHeight);

  std::vector<IDXGIAdapter1Ptr> adapters;
  m_renderer->EnumerateAdapters([&adapters](IDXGIAdapter1Ptr a) { adapters.push_back(a); });

  CHAR  filepath[MAX_PATH];
  DWORD filepathLength = GetModuleFileNameA(NULL, filepath, MAX_PATH);

  std::basic_string<TCHAR> exeFile(filepath, filepathLength);
  const std::filesystem::path shaderPath = std::filesystem::path(exeFile).parent_path() / "Shader";

  for (IDXGIAdapter1Ptr adapter : adapters) {
    DXGI_ADAPTER_DESC1 desc;
    memset(&desc, 0, sizeof(desc));

    adapter->GetDesc1(&desc);
    if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
      continue;

    m_renderer->CreateDevice(Wnd(), adapter, shaderPath.wstring());
    break;
  }

  const std::filesystem::path fontsPath = std::filesystem::path(exeFile).parent_path() / "Fonts";
  m_renderer->GetFontManager()->LoadFont((fontsPath / "arial_6.spritefont").string(), "Arial", 6, TileEngine::FONTSTYLE_REGULAR);
  m_renderer->GetFontManager()->LoadFont((fontsPath / "arial_8.spritefont").string(), "Arial", 8, TileEngine::FONTSTYLE_REGULAR);
  m_renderer->GetFontManager()->LoadFont((fontsPath / "arial_10.spritefont").string(), "Arial", 10, TileEngine::FONTSTYLE_REGULAR);

  SetFocus(Wnd());
  ShowWindow(Wnd(), cmdshow);
  UpdateWindow(Wnd());

  RECT rc;
  GetClientRect(Wnd(), &rc);
  m_screenWidth = rc.right - rc.left;
  m_screenHeight = rc.bottom - rc.top;

  m_self = shared_from_this();
}

HWND D3dUI::Wnd() {
  return m_hWnd;
}

HINSTANCE D3dUI::Hinst() {
  return m_hinst;
}

unsigned D3dUI::ScreenWidth() const {
  return m_screenWidth;
}

unsigned D3dUI::ScreenHeight() const {
  return m_screenHeight;
}

void D3dUI::Render() {
  m_renderer->Render();
}

void D3dUI::Destroy() {
  m_renderer.reset();

  DestroyWindow(Wnd());
  m_hWnd = 0;
  Free();
}

D3dUI::D3dUIPtr D3dUI::GetD3dUI() {
  return m_self;
}

TileEngine::D3d::RendererPtr D3dUI::GetRenderer() {
  return m_renderer;
}

TileEngine::FontManager::Ptr D3dUI::GetFontManager() {
  return m_renderer->GetFontManager();
}

void D3dUI::Free() {
  m_self.reset();
}

void D3dUI::InitWindow(int cmdshow, unsigned windowWidth, unsigned windowHeight) {
  WNDCLASS wc;
  memset(&wc, 0, sizeof(wc));

  wc.style = CS_DBLCLKS;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = Hinst();
  wc.hIcon = LoadIcon(Hinst(), MAKEINTATOM(IDI_APPLICATION));
  wc.hCursor = NULL;
  wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = gszMainWindowClass;

  if (!RegisterClass(&wc)) {
    throw std::runtime_error("RegisterClass() failed");
  }

  DWORD	exStyle;

  const bool exclusive = !(windowWidth && windowHeight);
  if (exclusive)
    exStyle = WS_EX_TOPMOST;
  else
    exStyle = WS_EX_APPWINDOW;

  HWND hwnd = FindWindow(gszMainWindowClass, gszMainWindowName);
  if (hwnd) {

    if (IsIconic(hwnd)) {
      ShowWindow(hwnd, SW_RESTORE);
    }
    SetForegroundWindow(hwnd);
    exit(0);
  }

  DWORD dwStyle = WS_POPUP | WS_VISIBLE;
  RECT windowRect = { 0, 0, windowWidth, windowHeight };

  int width = windowWidth;
  int height = windowHeight;
  if (AdjustWindowRect(&windowRect, dwStyle, FALSE)) {
    width = windowRect.right - windowRect.left;
    height = windowRect.bottom - windowRect.top;
  }

  m_hWnd = CreateWindowEx(
    exStyle,
    gszMainWindowClass,
    gszMainWindowName,
    dwStyle,
    100,
    100,
    width,
    height,
    NULL,
    NULL,
    Hinst(),
    NULL);

  if (!Wnd()) {
    throw std::runtime_error("RegisterClass() failed");
  }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
  D3dUI::D3dUIPtr d3dUI = D3dUI::GetD3dUI();
  if (!d3dUI) {
    // Error?
    return DefWindowProc(hwnd, iMsg, wParam, lParam);
    //PostQuitMessage(-1);
  }

  if (/*!gDone && */ d3dUI) { // TODO
    d3dUI->HandleWindowsMessage(hwnd, iMsg, wParam, lParam);
  }

  static bool swallowNextChar = false;

  switch (iMsg) {
  case WM_CHAR:
    if (!swallowNextChar) {
      d3dUI->HandleKeyPress(wParam, lParam);
    }
    swallowNextChar = false;
    break;

  case WM_KEYDOWN:
    switch (wParam) {
    case VK_F1:
    case VK_F2:
    case VK_F3:
    case VK_F4:
    case VK_F5:
    case VK_F6:
    case VK_F7:
    case VK_F8:
    case VK_F9:
      if (!(GetKeyState(VK_SHIFT) & 0x8000)) {
        d3dUI->HandleKeyPress(wParam - VK_F1 + '1' + 128, lParam);
      }
      break;
    case VK_F10:
      if (!(GetKeyState(VK_SHIFT) & 0x8000)) {
        d3dUI->HandleKeyPress(wParam - VK_F10 + '0' + 128, lParam);
      }
      break;
    case VK_F11:
      if (!(GetKeyState(VK_SHIFT) & 0x8000)) {
        d3dUI->HandleKeyPress('!' + 128, lParam);
      }
      break;
    case VK_F12:
      if (!(GetKeyState(VK_SHIFT) & 0x8000)) {
        d3dUI->HandleKeyPress('@' + 128, lParam);
      }
      break;
    case VK_TAB:
      d3dUI->HandleKeyPress('\t' + 128, lParam);
      swallowNextChar = true;
      return 0;
    case VK_RETURN:
      d3dUI->HandleKeyPress('\r' + 128, lParam);
      swallowNextChar = true;
      return 0;
    case VK_BACK:
      d3dUI->HandleKeyPress(8 + 128, lParam);
      swallowNextChar = true;
      return 0;
    case VK_UP:
    case VK_DOWN:
    case VK_LEFT:
    case VK_RIGHT:
      d3dUI->HandleKeyPress(wParam + 256, lParam);
      break;
    }
    break;
  case WM_SYSKEYDOWN:

    if (wParam == VK_F10) {
      if (!(GetKeyState(VK_SHIFT) & 0x8000)) {
        d3dUI->HandleKeyPress(wParam - VK_F10 + '0' + 128, lParam);
      }
    }
    break;
  case WM_CLOSE:
    if (hwnd != d3dUI->Wnd()) {
      break;
    }
    // TODO
    PostQuitMessage(0);
    //gDone = TRUE;
    //d3dUI->Destroy();
    return 0;

  case k_MSWHEEL_ROLLMSG:
  {
    sint16 dir = HIWORD(wParam);
    if (dir >= 0) dir = 1;
    if (dir < 0) dir = -1;
    d3dUI->HandleMouseWheel(dir);
  }

  case WM_VSCROLL:
  {
    sint16 scrollCode = LOWORD(wParam);
    if (scrollCode == SB_LINEDOWN) {
      d3dUI->HandleMouseWheel((sint16)-1);
    } else if (scrollCode == SB_LINEUP) {
      d3dUI->HandleMouseWheel((sint16)1);
    }
  }
  break;
  case WM_MOUSEWHEEL:
    d3dUI->HandleMouseWheel((sint16)HIWORD(wParam));
    break;
  }

  return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

} // namespace ui::d3d::Direct3d