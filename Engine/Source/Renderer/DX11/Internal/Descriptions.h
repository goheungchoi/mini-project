#pragma once
#include "Common.h"
#include <stdexcept>

DXGI_SWAP_CHAIN_DESC CreateSwapChainDesc(UINT width, UINT height, DXGI_FORMAT format, DXGI_USAGE usage,
                                         UINT bufferCount, UINT sampleDescCount, UINT sampleQuality, HWND outputWindow,
                                         BOOL windowed, DXGI_SWAP_EFFECT swapEffect,UINT flags)
{
  DXGI_SWAP_CHAIN_DESC desc{};
  desc.BufferCount = bufferCount;
  desc.BufferDesc.Format = format;
  desc.BufferDesc.Width = width;
  desc.BufferDesc.Height = height;
  desc.Windowed = windowed;
  desc.SampleDesc.Count = sampleDescCount;
  desc.SampleDesc.Quality = sampleQuality;
  desc.SwapEffect = swapEffect;
  desc.BufferUsage = usage;
  desc.Flags = flags;
  desc.OutputWindow = outputWindow;
  return desc;
}

class com_exception : public std::exception
{
public:
  com_exception(HRESULT hr) : result(hr)
  {
  }

  const char *what() const noexcept override
  {
    static char s_str[64] = {};
    sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
    return s_str;
  }

private:
  HRESULT result;
};

inline void HR_T(HRESULT hr)
{
  if (FAILED(hr))
  {
    throw com_exception(hr);
  }
}
