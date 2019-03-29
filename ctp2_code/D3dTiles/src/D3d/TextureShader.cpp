#include "stdafx.h"

#include <string.h>
#include <filesystem>
#include <stdexcept>

#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "D3dTiles/D3d/TextureShader.h"

namespace TileEngine::D3d {

TextureShader::TextureShader() {}

TextureShader::~TextureShader() {}

void TextureShader::Initialize(ID3D11DevicePtr device, const std::wstring &shaderPath) {
  // Initialize the vertex and pixel shaders.
  const std::filesystem::path shaderDir(shaderPath);
  InitializeShader(device, (shaderDir / L"D3dTiles.fx").wstring(), (shaderDir / L"D3dTiles.fx").wstring());
}

void TextureShader::InitializeShader(ID3D11DevicePtr device, const std::wstring &vsFilename, const std::wstring &psFilename) {
  InitVertexShader(device, vsFilename);
  InitPixelShader(device, psFilename);

  // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
  D3D11_BUFFER_DESC matrixBufferDesc;
  memset(&matrixBufferDesc, 0, sizeof(matrixBufferDesc));

  matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  HRESULT hr = device->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer.GetInterfacePtr());
  if (FAILED(hr)) {
    throw std::runtime_error("CreateBuffer() failed");
  }

  /*
  // Create a texture sampler state description.
  D3D11_SAMPLER_DESC samplerDesc;
  memset(&samplerDesc, 0, sizeof(samplerDesc));

  samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.MipLODBias = 0.0f;
  samplerDesc.MaxAnisotropy = 1;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  samplerDesc.BorderColor[0] = 0;
  samplerDesc.BorderColor[1] = 0;
  samplerDesc.BorderColor[2] = 0;
  samplerDesc.BorderColor[3] = 0;
  samplerDesc.MinLOD = 0;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  // Create the texture sampler state.
  result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
  if (FAILED(result)) {
    return false;
  }
  */
}

void TextureShader::InitVertexShader(ID3D11DevicePtr device, const std::wstring &vsFilename) {
  ID3DBlobPtr vsBlob = CompileShaderFromFile(vsFilename.c_str(), "VS", "vs_4_0");

  // Create the vertex shader
  HRESULT hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader.GetInterfacePtr());
  if (FAILED(hr)) {
    throw std::runtime_error("CreateVertexShader() failed");
  }

  // Define the input layout
  D3D11_INPUT_ELEMENT_DESC layout[] =
  {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };
  UINT numElements = ARRAYSIZE(layout);

  // Create the input layout
  hr = device->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(),
    vsBlob->GetBufferSize(), &m_vertexLayout.GetInterfacePtr());
  if (FAILED(hr)) {
    throw std::runtime_error("CreateInputLayout() failed");
  }
}

void TextureShader::InitPixelShader(ID3D11DevicePtr device, const std::wstring &psFilename) {
  ID3DBlobPtr vsBlob = CompileShaderFromFile(psFilename.c_str(), "PS", "ps_4_0");

  // Create the pixel shader
  HRESULT hr = device->CreatePixelShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_pixelShader.GetInterfacePtr());
  if (FAILED(hr)) {
    throw std::runtime_error("CreatePixelShader() failed");
  }
}

bool TextureShader::Render(ID3D11DeviceContextPtr deviceContext, unsigned indexCount,
  const DirectX::XMMATRIX &worldMatrix, const DirectX::XMMATRIX &viewMatrix, const DirectX::XMMATRIX &projectionMatrix,
  ID3D11ShaderResourceViewPtr texture) {
  // Set the shader parameters that it will use for rendering.
  const bool result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
  if (!result) {
    return false;
  }

  // Now render the prepared buffers with the shader.
  RenderShader(deviceContext, indexCount);

  return true;
}

bool TextureShader::SetShaderParameters(ID3D11DeviceContextPtr deviceContext,
  const DirectX::XMMATRIX &worldMatrix, const DirectX::XMMATRIX &viewMatrix, const DirectX::XMMATRIX &projectionMatrix,
  ID3D11ShaderResourceViewPtr texture) {
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  memset(&mappedResource, 0, sizeof(mappedResource));

  // Lock the constant buffer so it can be written to.
  HRESULT result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(result)) {
    return false;
  }

  // Get a pointer to the data in the constant buffer.
  MatrixBufferType *dataPtr = (MatrixBufferType*)mappedResource.pData;

  // Transpose the matrices to prepare them for the shader.
  const DirectX::XMMATRIX worldMatrixShader = DirectX::XMMatrixTranspose(worldMatrix);
  const DirectX::XMMATRIX viewMatrixShader = DirectX::XMMatrixTranspose(viewMatrix);
  const DirectX::XMMATRIX projectionMatrixShader = DirectX::XMMatrixTranspose(projectionMatrix);

  // Copy the matrices into the constant buffer.
  dataPtr->world = worldMatrixShader;
  dataPtr->view = viewMatrixShader;
  dataPtr->projection = projectionMatrixShader;

  // Unlock the constant buffer.
  deviceContext->Unmap(m_matrixBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  unsigned bufferNumber = 0;

  // Now set the constant buffer in the vertex shader with the updated values.
  deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer.GetInterfacePtr());

  // Set shader texture resource in the pixel shader.
  deviceContext->PSSetShaderResources(0, 1, &texture.GetInterfacePtr());

  return true;
}

void TextureShader::RenderShader(ID3D11DeviceContextPtr deviceContext, unsigned indexCount) {
  // Set the vertex input layout.
  deviceContext->IASetInputLayout(m_vertexLayout);

  // Set the vertex and pixel shaders that will be used to render this triangle.
  deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
  deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

  // Set the sampler state in the pixel shader.
  //deviceContext->PSSetSamplers(0, 1, &m_sampleState);

  // Render the triangle.
  deviceContext->DrawIndexed(indexCount, 0, 0);

  return;
}

ID3DBlobPtr CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel) {
  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;

  // Disable optimizations to further improve shader debugging
  dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

  ID3DBlobPtr outBlob, errorBlob;
  HRESULT hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
    dwShaderFlags, 0, &outBlob.GetInterfacePtr(), &errorBlob.GetInterfacePtr());
  if (FAILED(hr)) {
    if (errorBlob) {
      OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
      errorBlob.Release();
    }
    throw std::runtime_error("D3DCompileFromFile() failed");
  }

  return outBlob;
}
} // namespace TileEngine::D3d
