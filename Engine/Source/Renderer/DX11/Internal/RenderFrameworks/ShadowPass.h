#pragma once
#include "../Common.h"
#include "../Descriptions.h"
#include "../Device.h"
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;
class ShadowPass
{
private:
  // 8192 8192
  ComPtr<ID3D11Texture2D> _shadowMap = nullptr;
  ComPtr<ID3D11ShaderResourceView> _shadowSRV = nullptr;
  ComPtr<ID3D11DepthStencilView> _shadowDSV = nullptr;
  D3D11_VIEWPORT _shadowViewPort = {};
  Device* _device = nullptr;

public:
  Matrix view;
  Matrix Projection;

public:
  /**
   * @brief Make shadow depth Texture and srv, dsv
   * @param device
   * @param width ShadowMap Width
   * @param height ShadowMap height
   */
  ShadowPass(Device* device, int width, int height) : _device{device}
  {
    UINT flags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    D3D11_TEXTURE2D_DESC texDesc = CreateTexture2DDesc(
        width, height, DXGI_FORMAT_R32_TYPELESS, 1, flags, 1);
    HR_T(_device->GetDevice()->CreateTexture2D(&texDesc, NULL,
                                               _shadowMap.GetAddressOf()));
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    HR_T(_device->GetDevice()->CreateDepthStencilView(
        _shadowMap.Get(), &dsvDesc, _shadowDSV.GetAddressOf()));
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    HR_T(_device->GetDevice()->CreateShaderResourceView(
        _shadowMap.Get(), &srvDesc, _shadowSRV.GetAddressOf()));
    _shadowViewPort = {.TopLeftX = 0,
                       .TopLeftY = 0,
                       .Width = static_cast<float>(width),
                       .Height = static_cast<float>(height),
                       .MinDepth = 0.f,
                       .MaxDepth = 1.f};
  }
  ~ShadowPass() {}

public:
  void Prepare()
  {
    ID3D11RenderTargetView* nullrtv = nullptr;
    ID3D11DeviceContext* dc = _device->GetImmContext();
    dc->ClearDepthStencilView(_shadowDSV.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
    dc->OMSetRenderTargets(0, &nullrtv, _shadowDSV.Get());
    dc->RSSetViewports(1, &_shadowViewPort);
    //SWTODO : 이어서 작업하기. 
  }
};