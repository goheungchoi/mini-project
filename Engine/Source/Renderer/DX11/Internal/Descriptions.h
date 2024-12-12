#pragma once
#include "Common.h"

using namespace Microsoft::WRL;
D3D11_BUFFER_DESC CreateBufferDesc(UINT byteWidth, D3D11_USAGE usage, UINT bindFlag)
{
  D3D11_BUFFER_DESC desc{};
  desc.ByteWidth = byteWidth;
  desc.Usage = usage;
  desc.BindFlags = bindFlag;
  return desc;
}

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
///D3D11_APPEND_ALIGNED_ELEMENT
std::vector<D3D11_INPUT_ELEMENT_DESC> CreateInputLayouDesc()
{
  
}