#pragma once

#include <functional>

#include <windows.h>
#include <comip.h>
#include <comdef.h>

#include <d3d11.h>
#include <dxgi.h>

#include "D3d/D3dInterfaces.h"

namespace TileEngine {
  namespace D3d {

    typedef std::function<void(IDXGIAdapter1Ptr)> AdapterReceiver;

    class DXGIFactory
    {
    public:
      DXGIFactory();
      virtual ~DXGIFactory();

      IDXGIFactory1Ptr &GetFactory();
      void Enumerate(const AdapterReceiver &e);
      IDXGISwapChainPtr CreateSwapChain(ID3D11DevicePtr &device, DXGI_SWAP_CHAIN_DESC &sd);

    private:
      IDXGIFactory1Ptr m_factory;
    };

  } // namespace D3d
} // namespace TileEngine
