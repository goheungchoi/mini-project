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
  ComPtr<ID3D11Texture2D> _screenNormalMap;
  ComPtr<ID3D11RenderTargetView> _screenNormalRT;
  ComPtr<ID3D11ShaderResourceView> _screenNormalRTSrv;
  ComPtr<ID3D11Texture2D> _ssaoMap;
  ComPtr<ID3D11RenderTargetView> _ssaoRT;
  ComPtr<ID3D11ShaderResourceView> _ssaortSrv;
  float _clearColor[4] = {0.f, 0.f, 0.f, 1.f};

public:
  SSAOPass(Device* device) : _device{device}
  {
    _quad = new Quad::QuadFrame(device);
    // normal texture
    _quad->InitializeBuffers();
    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
    texDesc = CreateTexture2DDesc(
        kScreenWidth, kScreenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 1);
    HR_T(_device->GetDevice()->CreateTexture2D(
        &texDesc, nullptr, _screenNormalMap.GetAddressOf()));
    D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
    ZeroMemory(&rtDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    rtDesc.Format = texDesc.Format;
    rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtDesc.Texture2D.MipSlice = 0;
    HR_T(_device->GetDevice()->CreateRenderTargetView(
        _screenNormalMap.Get(), &rtDesc, _screenNormalRT.GetAddressOf()));
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    HR_T(_device->GetDevice()->CreateShaderResourceView(
        _screenNormalMap.Get(), &srvDesc, _screenNormalRTSrv.GetAddressOf()));
    // ssao texture
    ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
    texDesc = CreateTexture2DDesc(
        kScreenWidth, kScreenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 1);
    HR_T(_device->GetDevice()->CreateTexture2D(&texDesc, nullptr,
                                               _ssaoMap.GetAddressOf()));
    ZeroMemory(&rtDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    rtDesc.Format = texDesc.Format;
    rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtDesc.Texture2D.MipSlice = 0;
    HR_T(_device->GetDevice()->CreateRenderTargetView(_ssaoMap.Get(), &rtDesc,
                                                      _ssaoRT.GetAddressOf()));
    ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    HR_T(_device->GetDevice()->CreateShaderResourceView(
        _ssaoMap.Get(), &srvDesc, _ssaortSrv.GetAddressOf()));
  }
  ~SSAOPass() { SAFE_RELEASE(_quad); }

public:
#ifdef _DEBUG
  void UpdateVarialbe()
  {
    if (ImGui::Begin("ssao"))
    {
      ImTextureID imgID = (ImTextureID)(uintptr_t)_screenNormalRTSrv.Get();
      ImGui::Image(imgID, ImVec2(320, 180));
      imgID = (ImTextureID)(uintptr_t)_ssaortSrv.Get();
      ImGui::Image(imgID, ImVec2(320, 180));
    }
    ImGui::End();
  }
#endif
  void WritePrepare(BackBuffer* backBuffer)
  {
    ID3D11DeviceContext* dc = _device->GetImmContext();
    dc->OMSetRenderTargets(1, _screenNormalRT.GetAddressOf(), backBuffer->mainDSV.Get());
    dc->ClearRenderTargetView(_screenNormalRT.Get(), _clearColor);
  }

  void ReadPrepare()
  {
    ID3D11DeviceContext* dc = _device->GetImmContext();
    dc->OMSetRenderTargets(1, _ssaoRT.GetAddressOf(), nullptr);
    dc->ClearRenderTargetView(_ssaoRT.Get(), _clearColor);
    dc->PSSetShaderResources(17, 1, _screenNormalRTSrv.GetAddressOf());
  }
  void QuadDraw() 
  { 
    ID3D11DeviceContext* dc = _device->GetImmContext(); 
    unsigned int stride;
    unsigned int offset;
    stride = sizeof(Quad::Vertex);
    offset = 0;
    dc->IASetVertexBuffers(0, 1, _quad->_vertexBuffer.GetAddressOf(), &stride,
                           &offset);
    dc->IASetIndexBuffer(_quad->_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    dc->DrawIndexed(_quad->_indexCount, 0, 0);
  }
  void ClearSRV()
  {
    ID3D11DeviceContext* dc = _device->GetImmContext();
    ID3D11ShaderResourceView* srvs[2]{
        nullptr,
    };
    dc->PSSetShaderResources(17, 2, srvs);
  }
  void BlurHorizontalPrepare()
  {
    ID3D11DeviceContext* dc = _device->GetImmContext();
    dc->OMSetRenderTargets(1, _screenNormalRT.GetAddressOf(), nullptr);
    dc->ClearRenderTargetView(_screenNormalRT.Get(), _clearColor);
    dc->PSSetShaderResources(18, 1, _ssaortSrv.GetAddressOf());

  }
  void BlurVerticlePrepare()
  {
    
  }
};