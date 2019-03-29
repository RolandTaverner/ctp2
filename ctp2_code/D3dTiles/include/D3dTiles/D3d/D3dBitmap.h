#pragma once

#include <DirectXMath.h>

#include "D3dTiles/Geometry.h"

#include "D3dTiles/D3d/D3dInterfaces.h"

namespace TileEngine::D3d {

class D3dBitmap {
private:
  struct VertexType {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 texture;
  };

public:
  D3dBitmap();
  ~D3dBitmap();

  bool Initialize(ID3D11DevicePtr, unsigned screenWidth, unsigned screenHeight, 
    unsigned width, unsigned height);
  void Shutdown();
  bool Render(ID3D11DeviceContextPtr, const TileEngine::Position &position, float zLevel);

  unsigned GetIndexCount() const;

private:
  bool InitializeBuffers(ID3D11DevicePtr);
  void ShutdownBuffers();
  bool UpdateBuffers(ID3D11DeviceContextPtr, const TileEngine::Position &, float zLevel);
  void RenderBuffers(ID3D11DeviceContextPtr);

private:
  ID3D11BufferPtr m_vertexBuffer, m_indexBuffer;
  unsigned m_vertexCount, m_indexCount;

  // The D3dBitmap will need to maintain some extra information that a 3D model wouldn't such as the screen size, 
  // the bitmap size, and the last place it was rendered. 
  // We have added extra private variables here to track that extra information.
  unsigned m_screenWidth, m_screenHeight;
  unsigned m_bitmapWidth, m_bitmapHeight;
  unsigned m_previousPosX, m_previousPosY;
};

} // namespace TileEngine::D3d


