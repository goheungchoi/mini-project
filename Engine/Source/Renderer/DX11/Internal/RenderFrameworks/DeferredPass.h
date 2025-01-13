#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../RenderFrameworks/Shader.h"
#include "../Resources/Buffer.h"
#include "Core/Common.h"
#include "directxtk/SimpleMath.h"
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;
namespace Quad
{
struct Vertex
{
  Vector4 position;
  Vector2 texture;
};
class QuadFrame
{
private:
  ComPtr<ID3D11Buffer> _vertexBuffer;
  ComPtr<ID3D11Buffer> _indexBuffer;
  ComPtr<ID3D11Buffer> _constant;
  int _vertexCount, _indexCount;
  ID3D11DeviceContext* _dc = nullptr;

public:
  // SWTODO : 초기화
  QuadFrame() {}
  ~QuadFrame() {}

public:
  void InitializeBuffers(int windowWidth, int windowHeight) {}
};
} // namespace Quad

class DefferedPass
{
private:
  Device* _device;
  Quad::QuadFrame _frame;
  // 0 : depth,1 : albedo, 2 : Normal, 3 : Material(metalic,roughness), 4 :
  // Emissive 5: shadowPosition
  std::vector<ComPtr<ID3D11Texture2D>> _renderTargetTextures;
  std::vector<ComPtr<ID3D11RenderTargetView>> _renderTargets;
  std::vector<ComPtr<ID3D11ShaderResourceView>> _renderTargetSRVs;
  size_t _gBuffSize = 6;
  float _clearColor[4] = {0.f, 0.f, 0.f, 1.f};

public:
  DefferedPass(int width, int hegiht, Device* device) : _device{device}
  {
    _renderTargetTextures.resize(_gBuffSize);
    _renderTargets.resize(_gBuffSize);
    _renderTargetSRVs.resize(_gBuffSize);
    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
    texDesc = CreateTexture2DDesc(
        width, hegiht, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 1);

    std::ranges::for_each(std::views::iota(0u, _gBuffSize), [&](size_t i) {
      HR_T(_device->GetDevice()->CreateTexture2D(
          &texDesc, nullptr, _renderTargetTextures[i].GetAddressOf()));
    });

    D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
    ZeroMemory(&rtDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    rtDesc.Format = texDesc.Format;
    rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtDesc.Texture2D.MipSlice = 0;
    std::ranges::for_each(std::views::iota(0u, _gBuffSize), [&](size_t i) {
      HR_T(_device->GetDevice()->CreateRenderTargetView(
          _renderTargetTextures[i].Get(), &rtDesc,
          _renderTargets[i].GetAddressOf()));
    });
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    std::ranges::for_each(std::views::iota(0u, _gBuffSize), [&](size_t i) {
      HR_T(_device->GetDevice()->CreateShaderResourceView(
          _renderTargetTextures[i].Get(), &srvDesc,
          _renderTargetSRVs[i].GetAddressOf()));
    });
  }
  ~DefferedPass() {}

public:
  void DrawMeshDeffered(MeshBuffer* buffer)
  {
    std::ranges::for_each(_renderTargets,
                          [&](ComPtr<ID3D11RenderTargetView>& targetView) {
                            _device->GetImmContext()->ClearRenderTargetView(
                                targetView.Get(), _clearColor);
                          });

    _device->GetImmContext()->OMSetRenderTargets(
        static_cast<UINT>(_renderTargets.size()),
        _renderTargets.data()->GetAddressOf(), nullptr);
    // SWTODO : 드로우 호출
  }
};