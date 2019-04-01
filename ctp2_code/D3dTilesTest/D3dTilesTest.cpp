// D3dTilesTest.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"

#include <memory>

#include "D3dTilesTest.h"

#include "D3dTiles/Bitmap.h"
#include "D3dTiles/Font.h"
#include "D3dTiles/Scene.h"

#include "D3dTiles/D3d/Renderer.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND g_hWnd;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR    lpCmdLine,
  _In_ int       nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Разместите код здесь.

  // Инициализация глобальных строк
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_MYD3DTILESTEST, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // Выполнить инициализацию приложения:
  if (!InitInstance(hInstance, nCmdShow))
  {
    return FALSE;
  }

  TileEngine::D3d::RendererPtr rendererPtr(std::make_shared<TileEngine::D3d::Renderer>());

  std::vector<IDXGIAdapter1Ptr> adapters;
  rendererPtr->EnumerateAdapters([&adapters](IDXGIAdapter1Ptr a) { adapters.push_back(a); });

  for (IDXGIAdapter1Ptr adapter : adapters)
  {
    DXGI_ADAPTER_DESC1 desc;
    memset(&desc, 0, sizeof(desc));

    adapter->GetDesc1(&desc);
    if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
      continue;

    rendererPtr->CreateDevice(g_hWnd, adapter, L"");

    break;
  }

  rendererPtr->LoadFont("arial.spritefont", "Arial", 32, TileEngine::FONTSTYLE_REGULAR);

  RECT rc;
  GetClientRect(g_hWnd, &rc);
  unsigned screenWidth = rc.right - rc.left;
  unsigned screenHeight = rc.bottom - rc.top;

  TileEngine::Bitmap::Ptr b1(std::make_shared<TileEngine::Bitmap>(screenWidth, screenHeight));
  {
    const unsigned width = b1->Width();
    const unsigned height = b1->Height();
    auto pixels = b1->GetPixelsForUpdate();
    memset(pixels.get(), 0xFF, b1->BufSize());
    for (unsigned i = 0; i < height * width; ++i) {
      pixels[i] = 0xffffffff;
    }
    for (unsigned x = 0; x < width; ++x) {
      unsigned y = (float)x * height / width;
      pixels[y*width + x] = (x < width/2) ? 0xff0000ff : 0xffff0000;
    }
  }
  TileEngine::Bitmap::Ptr b2(std::make_shared<TileEngine::Bitmap>(100, 100));
  {
    const unsigned width = b2->Width();
    const unsigned height = b2->Height();
    auto pixels = b2->GetPixelsForUpdate();
    memset(pixels.get(), 0x33, b2->BufSize());
    for (unsigned x = 0; x < width / 2; ++x) {
      for (unsigned y = 0; y < height / 2; ++y) {
        pixels[y*width + x] = 0xA000FFff;
      }
    }
    for (unsigned x = width / 2; x < width; ++x) {
      for (unsigned y = 0; y < height / 2; ++y) {
        pixels[y*width + x] = 0xA0990000;
      }
    }
  }

  TileEngine::Scene::Ptr scene(std::make_shared<TileEngine::Scene>(screenWidth, screenHeight));
  auto layer = scene->Root()->CreateLayer(0);
  layer->DrawPrimitive(TileEngine::Position(20, 20),
    std::make_shared<TileEngine::ColoredRectangle>(screenWidth - 40, screenHeight - 40, TileEngine::MakeColor(0, 200, 200, 100)));

  auto region = layer->CreateChild(TileEngine::Position(0, 0), screenWidth, screenHeight);
  region->DrawImage(TileEngine::Position(0, 0), b1);
  auto region1 = region->CreateLayer(1);
  region1->DrawImage(TileEngine::Position(20, 20), b2);

  auto layer2 = scene->Root()->CreateLayer(1);
  auto region3 = layer2->CreateChild(TileEngine::Position(30, 30), screenWidth - 60, screenHeight - 60);

  region3->DrawPrimitive(TileEngine::Position(0, 0),
    std::make_shared<TileEngine::ColoredRectangle>(region3->Width(), region3->Height(), TileEngine::MakeColor(0, 200, 200, 100)));

  TileEngine::Text::Ptr text = std::make_shared<TileEngine::Text>(
    rendererPtr->GetFontManager(),
    TileEngine::FontDesc("Arial", 32, TileEngine::FONTSTYLE_REGULAR), 
    "Test text", TileEngine::MakeColor(255, 100, 0, 0xFF));
  region3->DrawPrimitive(TileEngine::Position(0, 50), text);

  auto region4 = region3->CreateLayer();
  auto region5 = region3->CreateLayer();

  TileEngine::Text::Ptr text1 = std::make_shared<TileEngine::Text>(
    rendererPtr->GetFontManager(),
    TileEngine::FontDesc("Arial", 32, TileEngine::FONTSTYLE_REGULAR),
    "Test text with text area", TileEngine::MakeColor(0, 255, 0, 0xFF));
  region5->DrawPrimitive(TileEngine::Position(0, 100), text1);

  region4->DrawPrimitive(TileEngine::Position(0, 100),
    std::make_shared<TileEngine::ColoredRectangle>(text1->Width(), text1->Height(), TileEngine::MakeColor(00, 00, 70, 40)));

  TileEngine::Text::Ptr text2 = std::make_shared<TileEngine::Text>(
    rendererPtr->GetFontManager(),
    TileEngine::FontDesc("Arial", 32, TileEngine::FONTSTYLE_REGULAR),
    "Scaled text scale=0.5", TileEngine::MakeColor(0, 0, 0, 0xFF), 0.5f);
  region5->DrawPrimitive(TileEngine::Position(0, 150), text2);

  TileEngine::Text::Ptr text3 = std::make_shared<TileEngine::Text>(
    rendererPtr->GetFontManager(),
    TileEngine::FontDesc("Arial", 32, TileEngine::FONTSTYLE_REGULAR),
    "Scaled text scale=0.3", TileEngine::MakeColor(0, 0, 0, 0xFF), 0.3f);
  region5->DrawPrimitive(TileEngine::Position(0, 180), text3);

  rendererPtr->SetScene(scene);

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYD3DTILESTEST));

  MSG msg = { 0 };
  while (WM_QUIT != msg.message)
  {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      rendererPtr->Render();
      Sleep(15);
    }
  }
  return (int)msg.wParam;

  /*
  MSG msg;

  // Цикл основного сообщения:
  while (GetMessage(&msg, nullptr, 0, 0))
  {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      rendererPtr.Render();
    }
  }

  return (int)msg.wParam;*/
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYD3DTILESTEST));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MYD3DTILESTEST);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

  g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!g_hWnd)
  {
    return FALSE;
  }

  ShowWindow(g_hWnd, nCmdShow);
  UpdateWindow(g_hWnd);

  return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
  {
    int wmId = LOWORD(wParam);
    // Разобрать выбор в меню:
    switch (wmId)
    {
    case IDM_ABOUT:
      DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  break;
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
    EndPaint(hWnd, &ps);
  }
  break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);
  switch (message)
  {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}
