#pragma once
#include "Common.h"
#include "Descriptions.h"
using namespace Microsoft::WRL;

class SwapChain
{
public:
  SwapChain() = default;
  ~SwapChain() = default;
  SwapChain(const SwapChain&) = delete;
  SwapChain& operator=(const SwapChain&) = delete;
  SwapChain(SwapChain&&) = delete;
  SwapChain& operator=(SwapChain&&) = delete;

public:
  void Init(HWND* hwnd, ID3D11Device* device, UINT width, UINT height)
  {
    UINT deviceFlags = 0;
#ifdef _DEBUG
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
    DXGI_SWAP_CHAIN_DESC desc =
        CreateSwapChainDesc(width, height, DXGI_FORMAT_R8G8B8A8_UNORM,
                            DXGI_USAGE_RENDER_TARGET_OUTPUT, 1, 1, 0, *hwnd,
                            true, DXGI_SWAP_EFFECT_DISCARD, deviceFlags);
    ComPtr<IDXGIFactory> factory = nullptr;
    HR_T(CreateDXGIFactory(__uuidof(IDXGIFactory),
                           (void**)factory.GetAddressOf()));
    // create swapchain
    HR_T(factory->CreateSwapChain(device, &desc,
                                  _swapChain.GetAddressOf()));

		// Get the window handle.
    _hwnd = *hwnd;
  }

	/**
   * @author Goheung Choi
   * @brief Get the HWND handle bound to the swapchian.
   * @return If set correctly, it returns a window handle. Otherwise, nullptr
   * will be returned.
   */
  HWND GetWindowHandle() { return _hwnd; }
  IDXGISwapChain* GetSwapChain() { return _swapChain.Get(); }

private:
  HWND _hwnd{nullptr};
  ComPtr<IDXGISwapChain> _swapChain = nullptr;
};