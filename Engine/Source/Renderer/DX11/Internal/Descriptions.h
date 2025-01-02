#pragma once
#include "Common.h"

using namespace Microsoft::WRL;
D3D11_BUFFER_DESC CreateBufferDesc(UINT byteWidth, D3D11_USAGE usage,
                                   UINT bindFlag)
{
  D3D11_BUFFER_DESC desc{};
  desc.ByteWidth = byteWidth;
  desc.Usage = usage;
  desc.BindFlags = bindFlag;
  return desc;
}

DXGI_SWAP_CHAIN_DESC CreateSwapChainDesc(
    UINT width, UINT height, DXGI_FORMAT format, DXGI_USAGE usage,
    UINT bufferCount, UINT sampleDescCount, UINT sampleQuality,
    HWND outputWindow, BOOL windowed, DXGI_SWAP_EFFECT swapEffect, UINT flags)
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

std::vector<D3D11_INPUT_ELEMENT_DESC> CreateInputLayoutDesc(
    const std::vector<uint8_t>& vsData, const size_t& vsSize)
{
  Microsoft::WRL::ComPtr<ID3D11ShaderReflection> pReflector;
  D3DReflect(vsData.data(), vsSize, IID_ID3D11ShaderReflection,
             (void**)pReflector.GetAddressOf());

  D3D11_SHADER_DESC shaderDesc;
  pReflector->GetDesc(&shaderDesc);

  std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
  inputLayoutDesc.reserve(shaderDesc.InputParameters);

  for (size_t i = 0; i < shaderDesc.InputParameters; i++)
  {
    D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
    pReflector->GetInputParameterDesc(static_cast<UINT>(i), &paramDesc);

    // 우리가 inputlayout 만들때 해준 semantic이랑 정해주던거
    D3D11_INPUT_ELEMENT_DESC elemDesc{
        .SemanticName = paramDesc.SemanticName,
        .SemanticIndex = paramDesc.SemanticIndex,
        .InputSlot = 0,
        .AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
        .InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
        .InstanceDataStepRate = 0};
    if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
    {
      if (paramDesc.Mask == 1)
        elemDesc.Format = DXGI_FORMAT_R32_FLOAT;
      else if (paramDesc.Mask <= 3)
        elemDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
      else if (paramDesc.Mask <= 7)
        elemDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
      else if (paramDesc.Mask <= 15)
        elemDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    }
    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
    {
      if (paramDesc.Mask == 1)
        elemDesc.Format = DXGI_FORMAT_R32_UINT;
      else if (paramDesc.Mask <= 3)
        elemDesc.Format = DXGI_FORMAT_R32G32_UINT;
      else if (paramDesc.Mask <= 7)
        elemDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
      else if (paramDesc.Mask <= 15)
        elemDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
    }
    else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
    {
      if (paramDesc.Mask == 1)
        elemDesc.Format = DXGI_FORMAT_R32_SINT;
      else if (paramDesc.Mask <= 3)
        elemDesc.Format = DXGI_FORMAT_R32G32_SINT;
      else if (paramDesc.Mask <= 7)
        elemDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
      else if (paramDesc.Mask <= 15)
        elemDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
    }
    inputLayoutDesc.push_back(elemDesc);
  }
  return inputLayoutDesc;
}

D3D11_TEXTURE2D_DESC CreateTexture2DDesc(UINT width, UINT height,
                                         DXGI_FORMAT format, UINT miplevels,
                                         UINT bindFlag)
{
  D3D11_TEXTURE2D_DESC desc = {};
  desc.Width = width;
  desc.Height = height;
  desc.MipLevels = miplevels;
  desc.ArraySize = 1;
  desc.Format = format;
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = bindFlag;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;

  if (miplevels == 0)
  {
    desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
  }
  return desc;
}

D3D11_DEPTH_STENCIL_DESC CreateDepthStencilDesc(BOOL depthEnable,
                                                D3D11_DEPTH_WRITE_MASK mask,
                                                D3D11_COMPARISON_FUNC func,
                                                BOOL stencilEnable)
{
  D3D11_DEPTH_STENCIL_DESC desc;
  ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
  desc.DepthEnable = depthEnable;
  desc.DepthWriteMask = mask;
  desc.DepthFunc = func; // 가까운물체만 렌더
  desc.StencilEnable = stencilEnable;
  return desc;
}