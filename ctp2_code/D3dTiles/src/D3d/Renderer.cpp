#include "stdafx.h"

#include <string.h>
#include <string>

#include <directxcolors.h>

#include <d3d11.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/SpriteBatch.h>

#include "D3dTiles/Color.h"
#include "D3dTiles/D3d/D3dBitmap.h"
#include "D3dTiles/D3d/D3dCreateTexture.h"
#include "D3dTiles/D3d/Renderer.h"

namespace TileEngine::D3d {

DirectX::XMVECTORF32 ColorToDXColor(Color c) {
  const DirectX::XMVECTORF32 dxColor = { { { 
      GetComponent<Red>(c)   / 255.0f, 
      GetComponent<Green>(c) / 255.0f,
      GetComponent<Blue>(c)  / 255.0f,
      GetComponent<Alpha>(c) / 255.0f } } };
  return dxColor;
}

Renderer::Renderer() :
  m_screenWidth(0), m_screenHeight(0), m_levelsCount(0) {}

Renderer::~Renderer() {}

void Renderer::EnumerateAdapters(const AdapterReceiver &e) {
  m_dxgi.Enumerate(e);
}

void Renderer::CreateDevice(HWND hWnd, IDXGIAdapter1Ptr adapter, const std::wstring &shaderPath) {
  D3D_FEATURE_LEVEL featureLevels[] =
  {
      D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
  };
  UINT numFeatureLevels = ARRAYSIZE(featureLevels);

  //const D3D_FEATURE_LEVEL featureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
  //const UINT numL1evelsRequested = 1;
  D3D_FEATURE_LEVEL featureLevelsSupported;

  HRESULT hr = D3D11CreateDevice(adapter.GetInterfacePtr(),
    D3D_DRIVER_TYPE_UNKNOWN,
    nullptr,
    0,
    featureLevels,
    numFeatureLevels,
    D3D11_SDK_VERSION,
    &m_device.GetInterfacePtr(),
    &featureLevelsSupported,
    &m_deviceContext.GetInterfacePtr());
  if (FAILED(hr)) {
    throw std::runtime_error("D3D11CreateDevice() failed");
  }

  RECT rc;
  GetClientRect(hWnd, &rc);
  m_screenWidth = rc.right - rc.left;
  m_screenHeight = rc.bottom - rc.top;

  DXGI_SWAP_CHAIN_DESC sd;
  memset(&sd, 0, sizeof(sd));
  sd.BufferCount = 1;
  sd.BufferDesc.Width = ScreenWidth();
  sd.BufferDesc.Height = ScreenHeight();
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  sd.Windowed = TRUE;

  m_swapChain = m_dxgi.CreateSwapChain(m_device, sd);

  m_dxgi.GetFactory()->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

  ID3D11Texture2DPtr backBuffer;
  hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer.GetInterfacePtr());
  if (FAILED(hr)) {
    throw std::runtime_error("GetBuffer() failed");
  }

  // D3D11_RENDER_TARGET_VIEW_DESC desc;
  hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView.GetInterfacePtr());
  if (FAILED(hr)) {
    throw std::runtime_error("CreateRenderTargetView() failed");
  }
  backBuffer.Release();

  InitDepthStencilBuffer(ScreenWidth(), ScreenHeight());
  InitDepthStencilState();
  InitDepthStencilView();
  InitBlendStateAlphaBlend();

  m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView.GetInterfacePtr(), m_depthStencilView);

  // Setup the viewport
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)ScreenWidth();
  vp.Height = (FLOAT)ScreenHeight();
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  m_deviceContext->RSSetViewports(1, &vp);

  m_worldMatrix = DirectX::XMMatrixIdentity();
  m_orthoMatrix = DirectX::XMMatrixOrthographicLH(ScreenWidth(), ScreenHeight(), 2.0, 1.0);

  DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
  DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
  DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  m_viewMatrix = DirectX::XMMatrixLookAtLH(Eye, At, Up);

  m_textureShader.Initialize(m_device, shaderPath);

  m_fontManager = std::make_shared<D3dFontManager>(m_device);
}

void Renderer::LoadFont(const std::string &file, const std::string &fontFace, FontStyle style) {
  m_fontManager->LoadFont(file, fontFace, style);
}

void Renderer::InitDepthStencilBuffer(const UINT &width, const UINT &height) {
  D3D11_TEXTURE2D_DESC depthBufferDesc;
  memset(&depthBufferDesc, 0, sizeof(depthBufferDesc));

  // Set up the description of the depth buffer.
  depthBufferDesc.Width = width;
  depthBufferDesc.Height = height;
  depthBufferDesc.MipLevels = 1;
  depthBufferDesc.ArraySize = 1;
  depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthBufferDesc.SampleDesc.Count = 1;
  depthBufferDesc.SampleDesc.Quality = 0;
  depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depthBufferDesc.CPUAccessFlags = 0;
  depthBufferDesc.MiscFlags = 0;

  // Create the texture for the depth buffer using the filled out description.
  HRESULT hr = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer.GetInterfacePtr());
  if (FAILED(hr)) {
    throw std::runtime_error("CreateTexture2D() failed");
  }
}

void Renderer::InitDepthStencilState() {
  D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
  memset(&depthStencilDesc, 0, sizeof(depthStencilDesc));

  depthStencilDesc.DepthEnable = false;
  depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
  depthStencilDesc.StencilEnable = true;
  depthStencilDesc.StencilReadMask = 0xFF;
  depthStencilDesc.StencilWriteMask = 0xFF;
  depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  HRESULT hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState.GetInterfacePtr());
  if (FAILED(hr)) {
    throw std::runtime_error("CreateDepthStencilState() failed");
  }

  m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
}

void Renderer::InitDepthStencilView() {
  D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
  memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));

  // Set up the depth stencil view description.
  depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  depthStencilViewDesc.Texture2D.MipSlice = 0;

  // Create the depth stencil view.
  HRESULT hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView.GetInterfacePtr());
  if (FAILED(hr)) {
    throw std::runtime_error("CreateDepthStencilView() failed");
  }
}

void Renderer::InitBlendStateAlphaBlend() {
  D3D11_BLEND_DESC blendState;
  memset(&blendState, 0, sizeof(blendState));

  blendState.RenderTarget[0].BlendEnable = TRUE;
  blendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  blendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  blendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  blendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  blendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  blendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  blendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

  HRESULT hr = m_device->CreateBlendState(&blendState, &m_blendStateAlphaBlend.GetInterfacePtr());
  if (FAILED(hr)) {
    throw std::runtime_error("CreateBlendState() failed");
  }
}

void Renderer::Render() {
  // Begin scene
  // Just clear the backbuffer
  m_deviceContext->ClearRenderTargetView(m_renderTargetView, ::DirectX::Colors::DarkBlue);
  m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

  float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  m_deviceContext->OMSetBlendState(m_blendStateAlphaBlend, blendFactor, 0xffffffff);

  if (Scene()) {
    SetLevelsCount(Scene()->GetLevelsCount());

    Scene()->Render(shared_from_this());
  }

  // End scene
  HRESULT hr = m_swapChain->Present(0, 0);
  if (FAILED(hr)) {
    throw std::runtime_error("Present() failed");
  }

  m_oneFrameTexCache.Clear();
}

FontManager::Ptr Renderer::GetFontManager() {
  return m_fontManager;
}

void Renderer::RenderBitmap(unsigned level, const Rect &absRect, Bitmap::Ptr b) {
  const float zLevel = (float)level / (float)LevelsCount();

  ID3D11Texture2DPtr tex;
  if (b->IsStatic()) {
    const std::string &id = b->StaticID();
    tex = GetFromTextureCache(id);
    if (!tex) {
      tex = Utils::CreateD3dTexture(m_device, *b);
      if (tex) {
        if (!AddToTextureCache(id, tex)) {
          // TODO: adding twice or non-unique StaticID
        }
      }
    }
  } else {
    tex = Utils::CreateD3dTexture(m_device, *b);
  }

  if (!tex) {
    throw std::runtime_error("CreateD3dTexture() failed");
  }

  ID3D11ShaderResourceViewPtr srv = Utils::CreateTexture2DSRV(m_device, tex);

  D3dBitmap bitmap;
  bitmap.Initialize(m_device, ScreenWidth(), ScreenHeight(), b->Width(), b->Height());
  bitmap.Render(m_deviceContext, absRect.min_corner(), zLevel);

  const bool result = m_textureShader.Render(m_deviceContext, bitmap.GetIndexCount(),
    m_worldMatrix, m_viewMatrix, m_orthoMatrix,
    srv);
}

void Renderer::RenderColoredRectangle(unsigned level, const Position &position, ColoredRectangle::Ptr p) {
  const float zLevel = (float)level / (float)LevelsCount();

  ID3D11Texture2DPtr tex;
  const std::string id = std::to_string(p->GetColor());
  tex = GetFromFrameTextureCache(id);
  if (!tex) {
    tex = Utils::CreateD3dTexture(m_device, p->GetColor());
    if (tex) {
      if (!AddToFrameTextureCache(id, tex)) {
        // TODO: adding twice or non-unique StaticID
      }
    }
  }

  if (!tex) {
    throw std::runtime_error("CreateD3dTexture() failed");
  }

  ID3D11ShaderResourceViewPtr srv = Utils::CreateTexture2DSRV(m_device, tex);

  D3dBitmap bitmap;
  bitmap.Initialize(m_device, ScreenWidth(), ScreenHeight(), p->Width(), p->Height());
  bitmap.Render(m_deviceContext, position, zLevel);

  const bool result = m_textureShader.Render(m_deviceContext, bitmap.GetIndexCount(),
    m_worldMatrix, m_viewMatrix, m_orthoMatrix,
    srv);
}

void Renderer::RenderTexturedRectangle(unsigned level, const Position &position, TexturedRectangle::Ptr p) {
  const float zLevel = (float)level / (float)LevelsCount();
  // TODO
}

void Renderer::RenderText(unsigned level, const Position &position, Text::Ptr text) {
  const float zLevel = (float)level / (float)LevelsCount();
  
  D3dFont::Ptr font = m_fontManager->GetD3dFont(text->GetFontFace(), text->GetFontStyle());
  
  const DirectX::SimpleMath::Vector2 fontPos(position.get<0>(), position.get<1>());
  std::unique_ptr<DirectX::SpriteBatch> spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceContext.GetInterfacePtr());

  spriteBatch->Begin();

  font->GetSpriteFont().DrawString(spriteBatch.get(), text->GetText().c_str(),
    fontPos, 
    ColorToDXColor(text->GetTextColor()), 
    0.f,                     // rotation
    DirectX::XMFLOAT2(0, 0), // origin
    1,                       // scale
    DirectX::SpriteEffects_None, 
    zLevel);

  spriteBatch->End();
}


unsigned Renderer::ScreenWidth() const {
  return m_screenWidth;
}

unsigned Renderer::ScreenHeight() const {
  return m_screenHeight;
}

void Renderer::SetLevelsCount(unsigned levels) {
  m_levelsCount = levels;
}

unsigned Renderer::LevelsCount() const {
  return m_levelsCount;
}

ID3D11Texture2DPtr Renderer::GetFromTextureCache(const std::string &id) {
  return m_texCache.TryGet(id);
}

bool Renderer::AddToTextureCache(const std::string &id, ID3D11Texture2DPtr texture) {
  return m_texCache.Set(id, texture);
}

ID3D11Texture2DPtr Renderer::GetFromFrameTextureCache(const std::string &id) {
  return m_oneFrameTexCache.TryGet(id);
}

bool Renderer::AddToFrameTextureCache(const std::string &id, ID3D11Texture2DPtr texture) {
  return m_oneFrameTexCache.Set(id, texture);
}

} // namespace TileEngine::D3d
