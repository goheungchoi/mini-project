#pragma once
#include "Common.h"
#include "Descriptions.h"
using namespace Microsoft::WRL;

class Device
{
public:
  Device() = default;
  ~Device() = default;
  Device(const Device&) = delete;
  Device& operator=(const Device&) = delete;
  Device(Device&&) = delete;
  Device& operator=(Device&&) = delete;

public:
  void CreateDevice(HWND* hwnd)
  {
    UINT deviceFlags = 0;
#ifdef _DEBUG
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
    const D3D_FEATURE_LEVEL featurelevel = D3D_FEATURE_LEVEL_11_1;
    // create device and immediate Context
    HR_T(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                           deviceFlags, &featurelevel, 1, D3D11_SDK_VERSION,
                           _device.GetAddressOf(), nullptr,
                           _immediateContext.GetAddressOf()));
  }
  ID3D11Device* GetDevice() { return _device.Get(); }
  ID3D11DeviceContext* GetImmContext() { return _immediateContext.Get(); }

private:
  ComPtr<ID3D11Device> _device = nullptr;
  ComPtr<ID3D11DeviceContext> _immediateContext = nullptr;
  //test
  //test
  //test
  // test
};
