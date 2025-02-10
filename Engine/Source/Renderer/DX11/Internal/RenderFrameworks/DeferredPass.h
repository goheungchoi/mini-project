#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../RenderFrameworks/Shader.h"
#include "../Resources/Buffer.h"
#include "../Resources/PipeLineState.h"
#include "../Resources/QuadFrame.h"
#include "Core/Common.h"
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;

class DefferedPass
{
private:
  Device* _device;
  Quad::QuadFrame* _frame;
  // 0: albedo,depth 1 : Normal, 2: Material(metalic,roughness), 3 :
  // Emissive 4: shadowPosition
  std::vector<ComPtr<ID3D11Texture2D>> _renderTargetTextures;
  std::vector<ComPtr<ID3D11RenderTargetView>> _renderTargets;
  std::vector<ComPtr<ID3D11ShaderResourceView>> _renderTargetSRVs;
  size_t _gBuffSize = 5;
  float _clearColor[4] = {0.1568f, 0.1568f, 0.1568f, 1.f};
  float _clearColor2[4] = {0.2f, 0.2f, 0.2f, 1.f};

public:
  DefferedPass(int width, int height, Device* device) : _device{device}
  {
    _frame = new Quad::QuadFrame(device);
    _frame->InitializeBuffers();
    _renderTargetTextures.resize(_gBuffSize);
    _renderTargets.resize(_gBuffSize);
    _renderTargetSRVs.resize(_gBuffSize);
    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
    texDesc = CreateTexture2DDesc(
        width, height, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,
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
  ~DefferedPass() { SAFE_RELEASE(_frame); }

public:
  void Prepare(BackBuffer* backBuffer)
  {
    _device->GetImmContext()->OMSetRenderTargets(
        static_cast<UINT>(_renderTargets.size()),
        _renderTargets.data()->GetAddressOf(), backBuffer->mainDSV.Get());
  }
  void ClearRenderTargets()
  {

    std::ranges::for_each(_renderTargets,
                          [&](ComPtr<ID3D11RenderTargetView>& targetView) {
                            _device->GetImmContext()->ClearRenderTargetView(
                                targetView.Get(), _clearColor);
                          });
  }
  void QuadDraw()
  {
    unsigned int stride;
    unsigned int offset;
    // Set vertex buffer stride and offset.
    stride = sizeof(Quad::Vertex);
    offset = 0;
    //_dc->UpdateSubresource(_constant.Get(), 0, nullptr, &cb, 0, 0);
    _device->GetImmContext()->IASetVertexBuffers(
        0, 1, _frame->_vertexBuffer.GetAddressOf(), &stride, &offset);
    _device->GetImmContext()->IASetIndexBuffer(_frame->_indexBuffer.Get(),
                                               DXGI_FORMAT_R32_UINT, 0);
    _device->GetImmContext()->PSSetShaderResources(
        10, _gBuffSize, _renderTargetSRVs.data()->GetAddressOf());
    _device->GetImmContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _device->GetImmContext()->DrawIndexed(_frame->_indexCount, 0, 0);
#ifdef _DEBUG
    if (ImGui::Begin("Deffered"))
    {
      ImTextureID imgID = (ImTextureID)(uintptr_t)_renderTargetSRVs[0].Get();
      ImGui::Image(imgID, ImVec2(240, 135));
      imgID = (ImTextureID)(uintptr_t)_renderTargetSRVs[1].Get();
      ImGui::Image(imgID, ImVec2(240, 135));
      imgID = (ImTextureID)(uintptr_t)_renderTargetSRVs[2].Get();
      ImGui::Image(imgID, ImVec2(240, 135));
      imgID = (ImTextureID)(uintptr_t)_renderTargetSRVs[3].Get();
      ImGui::Image(imgID, ImVec2(240, 135));
      imgID = (ImTextureID)(uintptr_t)_renderTargetSRVs[4].Get();
      ImGui::Image(imgID, ImVec2(240, 135));
    }
    ImGui::End();
#endif
  }
  void ClearSrvs()
  {
    ID3D11ShaderResourceView* srvs[6] = {
        nullptr,
    };
    _device->GetImmContext()->PSSetShaderResources(10, 6, srvs);
  }

  void DrawVinette()
  {
    ID3D11DeviceContext* dc = _device->GetImmContext();
    dc->OMSetRenderTargets(1, _renderTargets[0].GetAddressOf(), nullptr);
    unsigned int stride = sizeof(Quad::Vertex);
    unsigned int offset = 0;
    dc->IASetVertexBuffers(0, 1, _frame->_vertexBuffer.GetAddressOf(), &stride,
                           &offset);
    dc->IASetIndexBuffer(_frame->_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _device->GetImmContext()->DrawIndexed(_frame->_indexCount, 0, 0);
  }
};
