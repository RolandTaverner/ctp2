#pragma once

#include <string>

#include <windows.h>

#include <d3d11.h>
#include <DirectXMath.h>

#include "D3d/D3dInterfaces.h"

namespace TileEngine::D3d {

class TextureShader {
private:
private:
  typedef struct {
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
  } MatrixBufferType;

public:
  TextureShader();
  ~TextureShader();

  void Initialize(ID3D11DevicePtr device);
  bool Render(ID3D11DeviceContextPtr deviceContext, unsigned indexCount,
    const DirectX::XMMATRIX &worldMatrix, const DirectX::XMMATRIX &viewMatrix, const DirectX::XMMATRIX &projectionMatrix,
    ID3D11ShaderResourceViewPtr texture);

private:
  void InitializeShader(ID3D11DevicePtr device, const std::wstring &vsFilename, const std::wstring &psFilename);
  void InitVertexShader(ID3D11DevicePtr device, const std::wstring &vsFilename);
  void InitPixelShader(ID3D11DevicePtr device, const std::wstring &psFilename);

  bool SetShaderParameters(ID3D11DeviceContextPtr deviceContext, 
    const DirectX::XMMATRIX &worldMatrix, const DirectX::XMMATRIX &viewMatrix, const DirectX::XMMATRIX &projectionMatrix, 
    ID3D11ShaderResourceViewPtr);

  void RenderShader(ID3D11DeviceContextPtr deviceContext, unsigned);

private:
  ID3D11VertexShaderPtr m_vertexShader;
  ID3D11InputLayoutPtr m_vertexLayout;
  ID3D11PixelShaderPtr m_pixelShader;
  ID3D11BufferPtr m_matrixBuffer;
  //ID3D11SamplerState* m_sampleState;
};

ID3DBlobPtr CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel);

} // namespace TileEngine::D3d

