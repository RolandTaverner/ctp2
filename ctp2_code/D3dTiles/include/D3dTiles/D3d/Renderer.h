#pragma once

#include <memory>

#include <boost/scoped_array.hpp>

#include <d3d11.h>
#include <DirectXMath.h>

#include "D3dTiles/RendererBase.h"

#include "D3dTiles/D3d/DXGIFactory.h"
#include "D3dTiles/D3d/D3dFontManager.h"
#include "D3dTiles/D3d/D3dInterfaces.h"
#include "D3dTiles/D3d/D3dTextureCache.h"
#include "D3dTiles/D3d/TextureShader.h"

namespace TileEngine {
namespace D3d {

struct TileVertex {
  DirectX::XMFLOAT3 Pos;
  DirectX::XMFLOAT2 Tex;
};

class Renderer :
  public TileEngine::RendererBase {
public:
  Renderer();
  virtual ~Renderer();

  void EnumerateAdapters(const AdapterReceiver &e);
  void CreateDevice(HWND hWnd, IDXGIAdapter1Ptr adapter, const std::wstring &shaderPath);
  void LoadFont(const std::string &file, const std::string &fontFace, FontStyle style);
  void Render();

  // RendererBase
  FontManager::Ptr GetFontManager() override;
  void RenderBitmap(unsigned level, const Rect &absRect, Bitmap::Ptr s) override;
  void RenderColoredRectangle(unsigned level, const Position &position, ColoredRectangle::Ptr p) override;
  void RenderTexturedRectangle(unsigned level, const Position &position, TexturedRectangle::Ptr p) override;
  void RenderText(unsigned level, const Position &position, Text::Ptr p) override;

  unsigned ScreenWidth() const;
  unsigned ScreenHeight() const;

private:
  // Init functions
  void InitDepthStencilState();
  void InitDepthStencilBuffer(const UINT &width, const UINT &height);
  void InitDepthStencilView();
  void InitBlendStateAlphaBlend();

  // Render helper functions
  void SetLevelsCount(unsigned levels);
  unsigned LevelsCount() const;

  ID3D11Texture2DPtr GetFromTextureCache(const std::string &id);
  bool AddToTextureCache(const std::string &id, ID3D11Texture2DPtr texture);

  ID3D11Texture2DPtr GetFromFrameTextureCache(const std::string &id);
  bool AddToFrameTextureCache(const std::string &id, ID3D11Texture2DPtr texture);

private:
  unsigned m_screenWidth, m_screenHeight;

  DXGIFactory m_dxgi;
  IDXGISwapChainPtr m_swapChain;
  ID3D11DevicePtr m_device;
  ID3D11DeviceContextPtr m_deviceContext;
  ID3D11RenderTargetViewPtr m_renderTargetView;
  ID3D11Texture2DPtr m_depthStencilBuffer;
  ID3D11DepthStencilStatePtr m_depthStencilState;
  ID3D11DepthStencilViewPtr m_depthStencilView;
  ID3D11BlendStatePtr m_blendStateAlphaBlend;

  DirectX::XMMATRIX m_worldMatrix;
  DirectX::XMMATRIX m_orthoMatrix;
  DirectX::XMMATRIX m_viewMatrix;

  unsigned m_levelsCount;
  TextureCache m_texCache;
  TextureCache m_oneFrameTexCache;
  TextureShader m_textureShader;
  
  D3dFontManager::Ptr m_fontManager;
};

typedef std::shared_ptr<Renderer> RendererPtr;

} // namespace D3d
} // namespace TileEngine

