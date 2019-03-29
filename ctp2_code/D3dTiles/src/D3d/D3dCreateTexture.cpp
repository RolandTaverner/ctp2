#include "stdafx.h"

#include <string.h>
#include <cstdint>

#include "D3dTiles/D3d/D3dCreateTexture.h"

namespace TileEngine::D3d::Utils {

ID3D11Texture2DPtr CreateD3dTexture(ID3D11DevicePtr device, const TileEngine::Bitmap &s) {
  const unsigned bpp = sizeof(std::uint32_t);

  // setting up D3D11_SUBRESOURCE_DATA 
  D3D11_SUBRESOURCE_DATA tbsd;
  memset(&tbsd, 0, sizeof(tbsd));

  tbsd.pSysMem = (void *)s.GetPixels().get();
  tbsd.SysMemPitch = s.Width() * bpp;
  tbsd.SysMemSlicePitch = s.Width()*s.Height()*bpp; // Not needed since this is a 2d texture

  // setting up D3D11_TEXTURE2D_DESC 
  D3D11_TEXTURE2D_DESC tdesc;
  memset(&tdesc, 0, sizeof(tdesc));

  tdesc.Width = s.Width();
  tdesc.Height = s.Height();
  tdesc.MipLevels = 1;
  tdesc.ArraySize = 1;
  tdesc.SampleDesc.Count = 1;
  tdesc.SampleDesc.Quality = 0;
  tdesc.Usage = D3D11_USAGE_DEFAULT;
  tdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  tdesc.CPUAccessFlags = 0;
  tdesc.MiscFlags = 0;

  // create the texture
  ID3D11Texture2DPtr tex;
  if (FAILED(device->CreateTexture2D(&tdesc, &tbsd, &tex.GetInterfacePtr()))) {
    return ID3D11Texture2DPtr();
  }

  return tex;
}

ID3D11ShaderResourceViewPtr CreateTexture2DSRV(ID3D11DevicePtr device, ID3D11Texture2DPtr tex) {
  D3D11_TEXTURE2D_DESC tdesc;
  memset(&tdesc, 0, sizeof(tdesc));
  tex->GetDesc(&tdesc);

  D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
  memset(&SRVDesc, 0, sizeof(SRVDesc));

  SRVDesc.Texture2D.MipLevels = tdesc.MipLevels;
  SRVDesc.Texture2D.MostDetailedMip = 0;
  SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  SRVDesc.Format = tdesc.Format;

  ID3D11ShaderResourceViewPtr SRV;
  HRESULT hr = device->CreateShaderResourceView(tex, &SRVDesc, &SRV.GetInterfacePtr());
  if (FAILED(hr)) {
    return ID3D11ShaderResourceViewPtr();
  }
  return SRV;
}

} // namespace TileEngine::D3d::Utils
