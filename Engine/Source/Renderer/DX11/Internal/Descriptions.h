#pragma once
#include "Common.h"

using namespace Microsoft::WRL;
D3D11_BUFFER_DESC CreateBufferDesc(UINT size, D3D11_USAGE usage,
                                   D3D11_BIND_FLAG bindFlags,
                                   UINT structureStride = 0) 
{
  D3D11_BUFFER_DESC desc{};
  desc.Usage = usage;
  desc.ByteWidth = size;
  desc.BindFlags = bindFlags;
  desc.CPUAccessFlags =
      (usage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;
  desc.MiscFlags =
      structureStride > 0 ? D3D11_RESOURCE_MISC_BUFFER_STRUCTURED : 0;
  desc.StructureByteStride = structureStride;
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
void CreateInputLayoutDesc(
    std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDesc,
  const std::vector<uint8_t>& vsData,
                           const size_t& vsSize)
{
  Microsoft::WRL::ComPtr<ID3D11ShaderReflection> pReflector;
  D3DReflect(vsData.data(), vsSize, IID_ID3D11ShaderReflection,
             (void**)pReflector.GetAddressOf());

  D3D11_SHADER_DESC shaderDesc;
  pReflector->GetDesc(&shaderDesc);

  inputLayoutDesc.resize(shaderDesc.InputParameters);

  for (size_t i = 0; i < shaderDesc.InputParameters; i++)
  {
    D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
    pReflector->GetInputParameterDesc(static_cast<UINT>(i), &paramDesc);

    // 우리가 inputlayout 만들때 해준 semantic이랑 정해주던거
    D3D11_INPUT_ELEMENT_DESC elemDesc{
        .SemanticName =
            paramDesc.SemanticName ? _strdup(paramDesc.SemanticName) : nullptr,
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
    inputLayoutDesc[i] = elemDesc;
  }
}
// std::vector<D3D11_INPUT_ELEMENT_DESC> CreateInputLayoutDesc(
//     const std::vector<uint8_t>& vsData, const size_t& vsSize)
//{
//   Microsoft::WRL::ComPtr<ID3D11ShaderReflection> pReflector;
//   D3DReflect(vsData.data(), vsSize, IID_ID3D11ShaderReflection,
//              (void**)pReflector.GetAddressOf());
//
//   D3D11_SHADER_DESC shaderDesc;
//   pReflector->GetDesc(&shaderDesc);
//
//   std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
//   inputLayoutDesc.reserve(shaderDesc.InputParameters);
//
//   for (size_t i = 0; i < shaderDesc.InputParameters; i++)
//   {
//     D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
//     pReflector->GetInputParameterDesc(static_cast<UINT>(i), &paramDesc);
//
//     // 우리가 inputlayout 만들때 해준 semantic이랑 정해주던거
//     D3D11_INPUT_ELEMENT_DESC elemDesc{
//         .SemanticName = paramDesc.SemanticName,
//         .SemanticIndex = paramDesc.SemanticIndex,
//         .InputSlot = 0,
//         .AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
//         .InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
//         .InstanceDataStepRate = 0};
//     if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
//     {
//       if (paramDesc.Mask == 1)
//         elemDesc.Format = DXGI_FORMAT_R32_FLOAT;
//       else if (paramDesc.Mask <= 3)
//         elemDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
//       else if (paramDesc.Mask <= 7)
//         elemDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
//       else if (paramDesc.Mask <= 15)
//         elemDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//     }
//     else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
//     {
//       if (paramDesc.Mask == 1)
//         elemDesc.Format = DXGI_FORMAT_R32_UINT;
//       else if (paramDesc.Mask <= 3)
//         elemDesc.Format = DXGI_FORMAT_R32G32_UINT;
//       else if (paramDesc.Mask <= 7)
//         elemDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
//       else if (paramDesc.Mask <= 15)
//         elemDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
//     }
//     else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
//     {
//       if (paramDesc.Mask == 1)
//         elemDesc.Format = DXGI_FORMAT_R32_SINT;
//       else if (paramDesc.Mask <= 3)
//         elemDesc.Format = DXGI_FORMAT_R32G32_SINT;
//       else if (paramDesc.Mask <= 7)
//         elemDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
//       else if (paramDesc.Mask <= 15)
//         elemDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
//     }
//     inputLayoutDesc.push_back(elemDesc);
//   }
//   return inputLayoutDesc;
// }

D3D11_TEXTURE2D_DESC CreateTexture2DDesc(UINT width, UINT height,
                                         DXGI_FORMAT format, UINT miplevels,
                                         UINT bindFlag, UINT arraysize)
{
  D3D11_TEXTURE2D_DESC desc = {};
  desc.Width = width;
  desc.Height = height;
  desc.MipLevels = miplevels;
  desc.ArraySize = arraysize;
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
D3D11_RASTERIZER_DESC CreateRaterizerDesc(
    D3D11_FILL_MODE fillMode, // 채우기 모드
    D3D11_CULL_MODE cullMode, // 컬링 모드
    BOOL isCounterclockwise, INT DepthBias, FLOAT DepthBiasClamp,
    FLOAT SlopeScaledDepthBias, BOOL depthClipEnable, BOOL scissorEnable,
    BOOL multisampleEnable, BOOL antialiasedLineEnable)

{
  D3D11_RASTERIZER_DESC desc;
  ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
  desc.FillMode = fillMode;
  desc.CullMode = cullMode;
  desc.FrontCounterClockwise = isCounterclockwise;
  desc.DepthBias = DepthBias;
  desc.DepthBiasClamp = DepthBiasClamp;
  desc.SlopeScaledDepthBias = SlopeScaledDepthBias;
  desc.DepthClipEnable = depthClipEnable;
  desc.ScissorEnable = scissorEnable;
  desc.MultisampleEnable = multisampleEnable;
  desc.AntialiasedLineEnable = antialiasedLineEnable;
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

D3D11_RENDER_TARGET_BLEND_DESC CreateRTBlendDesc(BOOL blendEnable)
{
  D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
  rtBlendDesc.BlendEnable = blendEnable;    // 블렌딩 활성화
  rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD; // 블렌드 연산: 더하기
  rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA; // 소스 블렌드: 소스의 알파
  rtBlendDesc.DestBlend =
      D3D11_BLEND_INV_SRC_ALPHA; // 대상 블렌드: 소스 알파의 반전
  rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD; // 알파 블렌드 연산: 더하기
  rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;   // 소스 알파 블렌드: 1
  rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO; // 대상 알파 블렌드: 0
  rtBlendDesc.RenderTargetWriteMask =
      D3D11_COLOR_WRITE_ENABLE_ALL; // 모든 색상 채널에 쓰기 가능
  return rtBlendDesc;
}

// D3D11_SAMPLER_DESC CreateSamplerDesc(
//     D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE mode, UINT MaxAnisotropy,
//     D3D11_COMPARISON_FUNC ComparisonFunc, FLOAT MinLOD = 0,
//     FLOAT MaxLOD = D3D11_FLOAT32_MAX, FLOAT MipLODBias = 0)
//{
//   D3D11_SAMPLER_DESC sampleDesc = {};
//   sampleDesc.AddressU = mode;
//   sampleDesc.AddressV = mode;
//   sampleDesc.AddressW = mode;
//   sampleDesc.Filter = Filter;
//   sampleDesc.MaxAnisotropy = MaxAnisotropy;
//   sampleDesc.ComparisonFunc = ComparisonFunc;
//   sampleDesc.MinLOD = MinLOD;
//   sampleDesc.MaxLOD = MaxLOD;
//   sampleDesc.MipLODBias = MipLODBias;
//   return sampleDesc;
// }