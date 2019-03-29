#include "stdafx.h"

#include <string.h>

#include <boost/scoped_array.hpp>

#include "D3dTiles/D3d/D3dBitmap.h"

namespace TileEngine::D3d {

D3dBitmap::D3dBitmap() {
  m_vertexCount = m_indexCount = 0;
  m_screenWidth = m_screenHeight = 0;
  m_bitmapWidth = m_bitmapHeight = 0;
  m_previousPosX = m_previousPosY = 0;
}

D3dBitmap::~D3dBitmap() {
  Shutdown();
}

bool D3dBitmap::Initialize(ID3D11DevicePtr device, unsigned screenWidth, unsigned screenHeight, unsigned width, unsigned height) {
  /*
   In the Initialize function both the screen size and image size are stored.
   These will be required for generating exact vertex locations during rendering.
   Note that the pixels of the image do not need to be exactly the same as the texture that is used,
   you can set this to any size and use any size texture you want also.
  */

  // Store the screen size.
  m_screenWidth = screenWidth;
  m_screenHeight = screenHeight;

  // Store the size in pixels that this bitmap should be rendered at.
  m_bitmapWidth = width;
  m_bitmapHeight = height;
  /*
   The previous rendering location is first initialized to negative one.This will be an important variable
   that will locate where it last drew this image.If the image location hasn't changed since last frame
   then it won't modify the dynamic vertex buffer which will save us some cycles.
  */
  // Initialize the previous rendering position to negative one.
  m_previousPosX = -1;
  m_previousPosY = -1;
  // The buffers are then created and the texture for this bitmap image is also loaded in.

  // Initialize the vertex and index buffers.
  bool result = InitializeBuffers(device);
  if (!result) {
    return false;
  }

  return true;
}

// The Shutdown function will release the vertex and index buffers as well as the texture that was used for the bitmap image.
void D3dBitmap::Shutdown() {
  ShutdownBuffers();
}

// Render puts the buffers of the 2D image on the video card.
// As input it takes the position of where to render the image on the screen.
// The UpdateBuffers function is called with the position parameters.
// If the position has changed since the last frame it will then update the location of the vertices 
// in the dynamic vertex buffer to the new location.If not it will skip the UpdateBuffers function.
// After that the RenderBuffers function will prepare the final vertices / indices for rendering.
bool D3dBitmap::Render(ID3D11DeviceContextPtr deviceContext, const TileEngine::Position &position, float zLevel) {
  // Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
  if (!UpdateBuffers(deviceContext, position, zLevel)) {
    return false;
  }

  // Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
  RenderBuffers(deviceContext);

  return true;
}

// GetIndexCount returns the number of indexes for the 2D image.This will pretty much always be six.
unsigned D3dBitmap::GetIndexCount() const {
  return m_indexCount;
}

// InitializeBuffers is the function that is used to build the vertex and index buffer that will be used to draw the 2D image.
bool D3dBitmap::InitializeBuffers(ID3D11DevicePtr device) {
  // We set the vertices to six since we are making a square out of two triangles, so six points are needed.
  // The indices will be the same.
  // Set the number of vertices in the vertex array.
  m_vertexCount = 6;
  // Set the number of indices in the index array.
  m_indexCount = m_vertexCount;

  // Create the vertex array.
  boost::scoped_array<VertexType> vertices(new VertexType[m_vertexCount]);
  // Create the index array.
  boost::scoped_array<UINT32> indices(new UINT32[m_indexCount]);

  // Initialize vertex array to zeros at first.
  memset(vertices.get(), 0, (sizeof(VertexType)*m_vertexCount));

  // Load the index array with data.
  for (unsigned i = 0; i < m_indexCount; ++i) {
    indices[i] = i;
  }

  // Here is the big change in comparison to the ModelClass. We are now creating a dynamic vertex buffer 
  // so we can modify the data inside the vertex buffer each frame if we need to.
  // To make it dynamic we set Usage to D3D11_USAGE_DYNAMIC and CPUAccessFlags to D3D11_CPU_ACCESS_WRITE in the description.

  // Set up the description of the static vertex buffer.
  D3D11_BUFFER_DESC vertexBufferDesc;
  memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));

  vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  // Give the subresource structure a pointer to the vertex data.
  D3D11_SUBRESOURCE_DATA vertexData;
  memset(&vertexData, 0, sizeof(vertexData));

  vertexData.pSysMem = vertices.get();
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  // Now create the vertex buffer.
  HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer.GetInterfacePtr());
  if (FAILED(result)) {
    return false;
  }

  // We don't need to make the index buffer dynamic since the six indices will always point to the same six vertices 
  // even though the coordinates of the vertex may change.

  // Set up the description of the static index buffer.
  D3D11_BUFFER_DESC indexBufferDesc;
  memset(&indexBufferDesc, 0, sizeof(indexBufferDesc));

  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(UINT32)*m_indexCount;
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  // Give the subresource structure a pointer to the index data.
  D3D11_SUBRESOURCE_DATA indexData;
  memset(&indexData, 0, sizeof(indexData));

  indexData.pSysMem = indices.get();
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  // Create the index buffer.
  result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer.GetInterfacePtr());
  if (FAILED(result)) {
    return false;
  }

  return true;
}

// ShutdownBuffers releases the vertex and index buffers.
void D3dBitmap::ShutdownBuffers() {
  if (m_indexBuffer) {
    m_indexBuffer.Release();
  }

  if (m_vertexBuffer) {
    m_vertexBuffer.Release();
  }
}

// The UpdateBuffers function is called each frame to update the contents of the dynamic vertex buffer to re - position the 2D bitmap image on the screen if need be.
bool D3dBitmap::UpdateBuffers(ID3D11DeviceContextPtr deviceContext, const TileEngine::Position &position, float zLevel) {
  // We check if the position to render this image has changed.If it hasn't changed then we just exit since the vertex buffer doesn't need any changes for this frame.This check can save us a lot of processing.

  // If the position we are rendering this bitmap to has not changed then don't update the vertex buffer since it
  // currently has the correct parameters.
  if ((position.get<0>() == m_previousPosX) && (position.get<1>() == m_previousPosY)) {
    return true;
  }

  // If the position to render this image has changed then we record the new location for the next time we come through this function.

  // If it has changed then update the position it is being rendered to.
  m_previousPosX = position.get<0>();
  m_previousPosY = position.get<1>();

  // The four sides of the image need to be calculated.
  // See the diagram at the top of the tutorial for a complete explaination.

  // Calculate the screen coordinates of the left side of the bitmap.
  const float left = (float)((m_screenWidth/2.0) * -1.0) + (float)position.get<0>();
  // Calculate the screen coordinates of the right side of the bitmap.
  const float right = left + (float)m_bitmapWidth;
  // Calculate the screen coordinates of the top of the bitmap.
  const float top = (float)(m_screenHeight/2.0) - (float)position.get<1>();
  // Calculate the screen coordinates of the bottom of the bitmap.
  const float bottom = top - (float)m_bitmapHeight;

  // Now that the coordinates are calculated create a temporary vertex array and fill it with the new six vertex points.

  // Create the vertex array.
  boost::scoped_array<VertexType> vertices(new VertexType[m_vertexCount]);

  // Load the vertex array with data.
  // First triangle.
  vertices[0].position = DirectX::XMFLOAT3(left, top, zLevel);  // Top left.
  vertices[0].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

  vertices[1].position = DirectX::XMFLOAT3(right, bottom, zLevel);  // Bottom right.
  vertices[1].texture = DirectX::XMFLOAT2(1.0f, 1.0f);

  vertices[2].position = DirectX::XMFLOAT3(left, bottom, zLevel);  // Bottom left.
  vertices[2].texture = DirectX::XMFLOAT2(0.0f, 1.0f);

  // Second triangle.
  vertices[3].position = DirectX::XMFLOAT3(left, top, zLevel);  // Top left.
  vertices[3].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

  vertices[4].position = DirectX::XMFLOAT3(right, top, zLevel);  // Top right.
  vertices[4].texture = DirectX::XMFLOAT2(1.0f, 0.0f);

  vertices[5].position = DirectX::XMFLOAT3(right, bottom, zLevel);  // Bottom right.
  vertices[5].texture = DirectX::XMFLOAT2(1.0f, 1.0f);

  // Now copy the contents of the vertex array into the vertex buffer using the Map and memcpy functions.

  // Lock the vertex buffer so it can be written to.
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  memset(&mappedResource, 0, sizeof(mappedResource));

  HRESULT result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(result)) {
    return false;
  }

  // Get a pointer to the data in the vertex buffer.
  VertexType *verticesPtr = (VertexType*)mappedResource.pData;

  // Copy the data into the vertex buffer.
  memcpy(verticesPtr, vertices.get(), (sizeof(VertexType)*m_vertexCount));

  // Unlock the vertex buffer.
  deviceContext->Unmap(m_vertexBuffer, 0);

  return true;
}

// The RenderBuffers function sets up the vertex and index buffers on the gpu to be drawn by the shader.
void D3dBitmap::RenderBuffers(ID3D11DeviceContextPtr deviceContext) {
  // Set vertex buffer stride and offset.
  const UINT stride = sizeof(VertexType);
  const UINT offset = 0;

  // Set the vertex buffer to active in the input assembler so it can be rendered.
  deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer.GetInterfacePtr(), &stride, &offset);

  // Set the index buffer to active in the input assembler so it can be rendered.
  deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

  // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

} // namespace TileEngine::D3d

