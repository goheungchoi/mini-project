#pragma once
#include "Core/Common.h"
#include "Renderer/DX11/Internal/Common.h"
#include "Renderer/DX11/Internal/Descriptions.h"
#include "Renderer/DX11/Internal/Device.h"
#include "Renderer/DX11/Internal/Resources/Buffer.h"
#include "Renderer/DX11/Internal/Resources/QuadFrame.h"
class PostProcess
{
private:
  Device* _device;

protected:
  ComPtr<ID3D11RenderTargetView> _rtv;
  ComPtr<ID3D11Texture2D> _texture;
  ComPtr<ID3D11ShaderResourceView> _srv;

  Quad::QuadFrame* _frame;
  std::vector<RenderMesh::StaticMesh> _staticMeshes;
  std::vector<RenderMesh::SkelMesh> _skelMeshes;

public:
  PostProcess(Device* device) : _device{device}
  {
    _frame = new Quad::QuadFrame(device);
    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
    texDesc = CreateTexture2DDesc(
        kScreenWidth, kScreenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,
        D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 1);
    HR_T(_device->GetDevice()->CreateTexture2D(&texDesc, nullptr,
                                               _texture.GetAddressOf()));

    D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
    ZeroMemory(&rtDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    rtDesc.Format = texDesc.Format;
    rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtDesc.Texture2D.MipSlice = 0;
    HR_T(_device->GetDevice()->CreateRenderTargetView(_texture.Get(), &rtDesc,
                                                      _rtv.GetAddressOf()));
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    HR_T(_device->GetDevice()->CreateShaderResourceView(
        _texture.Get(), &srvDesc, _srv.GetAddressOf()));
  }
  virtual ~PostProcess()
  {
    SAFE_RELEASE(_frame);
    _staticMeshes.clear();
    _skelMeshes.clear();
  }
};