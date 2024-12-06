#pragma ocne
#include "Common.h"
#include "Descriptions.h"
using namespace Microsoft::WRL;

class Device
{
public:
  Device();
  ~Device() = default;

public:
  void CreateDevice(HWND* hwnd)
  {
    UINT deviceFlags = 0;
#ifdef _DEBUG
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
    const D3D_FEATURE_LEVEL featurelevel = D3D_FEATURE_LEVEL_11_1;
    HR_T(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, &featurelevel, 1, D3D11_SDK_VERSION,
                           device.GetAddressOf(), nullptr, deviceContext.GetAddressOf()));
   // DXGI_SWAP_CHAIN_DESC desc = CreateSwapChainDesc()
  }

  ID3D11Device *GetDevice()
  {
    return device.Get();
  }

  ID3D11DeviceContext *GetDeviceContext()
  {
    return deviceContext.Get();
  }

private:
  ComPtr<ID3D11Device> device;
  ComPtr<ID3D11DeviceContext> deviceContext;
  ComPtr<IDXGISwapChain> swapChain;
};
