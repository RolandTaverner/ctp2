#include "stdafx.h"

#include <string.h>

#include "D3d/DXGIFactory.h"

namespace TileEngine {
  namespace D3d {

    DXGIFactory::DXGIFactory()
    {
      if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&m_factory.GetInterfacePtr())))
      {
        throw std::runtime_error("CreateDXGIFactory1() failed");
      }
    }

    DXGIFactory::~DXGIFactory()
    {
    }

    IDXGIFactory1Ptr &DXGIFactory::GetFactory() {
      return m_factory;
    }

    void DXGIFactory::Enumerate(const AdapterReceiver &e) {
      IDXGIAdapter1Ptr adapter;
      for (unsigned i = 0; m_factory->EnumAdapters1(i, &adapter.GetInterfacePtr()) != DXGI_ERROR_NOT_FOUND; ++i)
      {
        e(adapter);
      }
    }

    IDXGISwapChainPtr DXGIFactory::CreateSwapChain(ID3D11DevicePtr &device, DXGI_SWAP_CHAIN_DESC &sd) {
      IDXGISwapChainPtr swapChain;

      HRESULT hr = m_factory->CreateSwapChain(device.GetInterfacePtr(), &sd, &swapChain.GetInterfacePtr());
      if (FAILED(hr)) {
        throw std::runtime_error("CreateSwapChain() failed");
      }

      return swapChain;
    }

  } // namespace D3d
} // namespace TileEngine
