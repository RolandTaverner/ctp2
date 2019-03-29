#pragma once

#include "D3dTiles/Bitmap.h"
#include "D3dTiles/D3d/D3dInterfaces.h"

namespace TileEngine::D3d::Utils {

ID3D11Texture2DPtr CreateD3dTexture(ID3D11DevicePtr device, const TileEngine::Bitmap &s);
ID3D11ShaderResourceViewPtr CreateTexture2DSRV(ID3D11DevicePtr device, ID3D11Texture2DPtr tex);

} // namespace TileEngine::D3d::Utils
