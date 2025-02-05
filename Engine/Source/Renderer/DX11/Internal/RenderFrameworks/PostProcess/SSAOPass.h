#pragma once
#include "Core/Common.h"
#include "Renderer/DX11/Internal/Common.h"
#include "Renderer/DX11/Internal/Device.h"
#include "Renderer/DX11/Internal/Resources/Buffer.h"
#include "Renderer/DX11/Internal/Resources/ConstantBuffer.h"
#include "Renderer/DX11/Internal/Resources/QuadFrame.h"
class SSAOPass
{
private:
  Quad::QuadFrame* _quad = nullptr;
  Device* _device = nullptr;
  ComPtr<ID3D11Texture2D> _rtTexture;
  ComPtr<ID3D11RenderTargetView> _rt;
  ComPtr<ID3D11ShaderResourceView> _rtSrv;
  ComPtr<ID3D11Texture2D> _depTexture;
  ComPtr<ID3D11ShaderResourceView> _depSrv;
  ComPtr<ID3D11DepthStencilView> _depDsv;
  float _clearColor[4] = {0.f, 0.f, 0.f, 1.f};

public:
  SSAOPass(Device* device) : _device{device}
  {
    _quad = new Quad::QuadFrame(device);
    _quad->InitializeBuffers();
    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
    texDesc = CreateTexture2DDesc(
        kScreenWidth, kScreenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 1);
    HR_T(_device->GetDevice()->CreateTexture2D(&texDesc, nullptr,
                                               _rtTexture.GetAddressOf()));
    D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
    ZeroMemory(&rtDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    rtDesc.Format = texDesc.Format;
    rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtDesc.Texture2D.MipSlice = 0;
    HR_T(_device->GetDevice()->CreateRenderTargetView(_rtTexture.Get(), &rtDesc,
                                                      _rt.GetAddressOf()));
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    HR_T(_device->GetDevice()->CreateShaderResourceView(
        _rtTexture.Get(), &srvDesc, _rtSrv.GetAddressOf()));
    ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
    texDesc = CreateTexture2DDesc(
        kScreenWidth, kScreenHeight, DXGI_FORMAT_R32_TYPELESS, 1,
        D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, 1);
    HR_T(_device->GetDevice()->CreateTexture2D(&texDesc, nullptr,
                                               _depTexture.GetAddressOf()));
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvdesc;
    ZeroMemory(&dsvdesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    dsvdesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    HR_T(_device->GetDevice()->CreateDepthStencilView(
        _depTexture.Get(), &dsvdesc, _depDsv.GetAddressOf()));
    ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    HR_T(_device->GetDevice()->CreateShaderResourceView(
        _depTexture.Get(), &srvDesc, _depSrv.GetAddressOf()));

  }
  ~SSAOPass() { SAFE_RELEASE(_quad); }

public:
#ifdef _DEBUG
  void UpdateVarialbe()
  {
    if (ImGui::Begin("ssao"))
    {
      ImTextureID imgID = (ImTextureID)(uintptr_t)_rtSrv.Get();
      ImGui::Image(imgID, ImVec2(400, 400));
      imgID = (ImTextureID)(uintptr_t)_depSrv.Get();
      ImGui::Image(imgID, ImVec2(400, 400));
    }
    ImGui::End();
  }
#endif
  void Prepare() 
  { 
    ID3D11DeviceContext* dc = _device->GetImmContext();
    dc->ClearRenderTargetView(_rt.Get(), _clearColor);
    dc->ClearDepthStencilView(_depDsv.Get(),
                              D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
    dc->OMSetRenderTargets(1, _rt.GetAddressOf(), _depDsv.Get());
  }
};