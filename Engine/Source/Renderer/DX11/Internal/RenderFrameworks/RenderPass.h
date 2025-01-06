#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../Objects/Swapchain.h"
#include "../Resources/FrameBuffer.h"
#include "../Resources/PipeLineState.h"
#include "../Resources/Sampler.h"
#include "../Types.h"
#include "Core/Common.h"
class RenderPassManager
{
private:
  std::vector<MeshBuffer*> _transparentMeshes;
  std::vector<MeshBuffer*> _shadowMesh;
  std::vector<MeshBuffer*> _opaqueMesh;
  FrameBuffer* _frameBuffer;
  std::vector<Sampler*> _samplers;

private:
  // renderer에서 생성한 deive 참조.
  Device* _device;
  PipeLine* _pso = nullptr;
  Matrix _view;
  Matrix _projection;

public:
  RenderPassManager(Device* device, SwapChain* swapchain, int width, int height)
      : _device{device}
  {
    _pso = new PipeLine(_device, swapchain, width, height);
    _device->GetImmContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _frameBuffer = new FrameBuffer(device);
  }
  ~RenderPassManager()
  {
    std::ranges::for_each(_samplers, [](Sampler* sam) { SAFE_RELEASE(sam); });
    SAFE_RELEASE(_frameBuffer);
    SAFE_RELEASE(_pso);
  }

public:
  void ClearBackBuffer() { _pso->ClearBackBuffer(_device); }
  void SetCameraMatrix(Matrix view, Matrix projection)
  {
    _view = view;
    _projection = projection;
  }
  void ClassfyPass(MeshBuffer* buff)
  {
    if (buff->flags & RenderPassType::Opaque)
    {
      _opaqueMesh.push_back(buff);
    }
    if (buff->flags & RenderPassType::Transparent)
    {
      _transparentMeshes.push_back(buff);
    }

    if (buff->flags & RenderPassType::Shadow)
    {
      _shadowMesh.push_back(buff);
    }
    if (buff->flags & RenderPassType::Light)
    {
    }
  }
  void ProcessPass()
  {
    // shadow pass

    // deffered pass

    // transparent -> forward rendering
    std::ranges::for_each(_transparentMeshes, [this](MeshBuffer* buffer) {
      _device->GetImmContext()->IASetVertexBuffers(
          0, 1, buffer->vertexBuffer.GetAddressOf(), &(buffer->stride),
          &(buffer->offset));
      _device->GetImmContext()->IASetIndexBuffer(buffer->indexBuffer.Get(),
                                                 DXGI_FORMAT_R32_UINT, 0);
      // TODO : shader handle 물어보고
      // 작업 끝나면 세팅해주기. ->각 메쉬별로들고있을건가?
      // _device->GetImmContext()->IASetInputLayout()
      Constant::WVP wvp = {buffer->world, _view, _projection};
      _frameBuffer->UpdateContantBuffer(wvp, CBType::WVP);
      _device->GetImmContext()->VSSetConstantBuffers(
          0, 1,
          _frameBuffer->_constantBuffers[static_cast<UINT>(CBType::WVP)]
              .GetAddressOf());
      //_device->GetImmContext()->VSSetShader();
      //_device->GetImmContext()->PSSetShader();
      //_device->GetImmContext()->PSSetShaderResources();
      _device->GetImmContext()->DrawIndexed(buffer->nIndices, 0, 0);
    });
  }

  void CreateSamplers()
  {
    D3D11_SAMPLER_DESC sampleDesc = {};
    // linear
    sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampleDesc.MinLOD = 0;
    sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
    MakeSampler(sampleDesc, SamplerType::LINEAR_WRAP);
    // iblspecularBRDF
    // clamp
    sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampleDesc.MaxAnisotropy = 1;
    sampleDesc.MinLOD = 0;
    sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
    MakeSampler(sampleDesc, SamplerType::LINEAR_CLAMP);
    // default
    // anisotropy
    sampleDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampleDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
    sampleDesc.MinLOD = 0;
    sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
    MakeSampler(sampleDesc, SamplerType::ANIMSOROPIC);
    // Comparison
    sampleDesc = {};
    sampleDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    sampleDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
    sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    MakeSampler(sampleDesc, SamplerType::COMPARISON);

    std::vector<ID3D11SamplerState*> samplers;
    std::ranges::for_each(_samplers, [&samplers](Sampler* sam) {
      samplers.push_back(sam->state.Get());
    });
    _device->GetImmContext()->PSSetSamplers(
        0, static_cast<UINT>(samplers.size()), samplers.data());
  }

private:
  void MakeSampler(D3D11_SAMPLER_DESC desc, SamplerType type)
  {
    Sampler* sam = new Sampler;
    sam->type = type;
    _device->GetDevice()->CreateSamplerState(&desc, sam->state.GetAddressOf());
    _samplers.push_back(sam);
  }
};