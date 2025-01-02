#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../SwapChain.h"
using namespace Microsoft::WRL;

class PipeLine
{
private:
  struct OutputMerger
  {
    ComPtr<ID3D11BlendState> blentState;
    ComPtr<ID3D11DepthStencilState> depthStencilState;
    ComPtr<ID3D11DepthStencilState> depthDisableStencilState;
  };

  struct Rasterizer
  {
    ComPtr<ID3D11RasterizerState> rasterizerState;
    D3D11_VIEWPORT viewPort;
  };

  struct BackBuffer
  {
    ComPtr<ID3D11RenderTargetView> mainRTV;
    ComPtr<ID3D11DepthStencilView> mainDSV;
  };

private:
  BackBuffer* _backBuffer = nullptr;
  OutputMerger* _om = nullptr;
  Rasterizer* _rs = nullptr;

public:
  PipeLine(Device* device, SwapChain* swapChain, int width, int height)
  {
    // alloc member
    _backBuffer = new BackBuffer;

    // 1. Output Merger Start
    _om = new OutputMerger;
    // main rendertarget.
    ComPtr<ID3D11Texture2D> pBackBufferTexture = nullptr;
    HR_T(swapChain->GetSwapChain()->GetBuffer(
        0, __uuidof(ID3D11Texture2D),
        (void**)(pBackBufferTexture.GetAddressOf())));
    HR_T(device->GetDevice()->CreateRenderTargetView(
        pBackBufferTexture.Get(), NULL, _backBuffer->mainRTV.GetAddressOf()));
    // main depth
    D3D11_TEXTURE2D_DESC depthTexdesc =
        CreateTexture2DDesc(width, height, DXGI_FORMAT_D24_UNORM_S8_UINT, 1,
                            D3D11_BIND_DEPTH_STENCIL);
    ComPtr<ID3D11Texture2D> pDepthTexture = nullptr;
    HR_T(device->GetDevice()->CreateTexture2D(&depthTexdesc, NULL,
                                              pDepthTexture.GetAddressOf()));
    HR_T(device->GetDevice()->CreateDepthStencilView(
        pDepthTexture.Get(), NULL, _backBuffer->mainDSV.GetAddressOf()));
    // depth stencil test state
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = CreateDepthStencilDesc(
        true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, false);
    D3D11_DEPTH_STENCIL_DESC depthDisalbeStencilDesc =
        CreateDepthStencilDesc( // for defferd
            false, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, false);
    device->GetDevice()->CreateDepthStencilState(
        &depthStencilDesc, _om->depthStencilState.GetAddressOf());
    device->GetDevice()->CreateDepthStencilState(
        &depthDisalbeStencilDesc, _om->depthDisableStencilState.GetAddressOf());
    device->GetImmContext()->OMSetDepthStencilState(
        _om->depthStencilState.Get(), // initial depth stencil state
        1);
    // set main render target
    device->GetImmContext()->OMSetRenderTargets(
        1, _backBuffer->mainRTV.GetAddressOf(), _backBuffer->mainDSV.Get());
    // Output Merger End
    //
    // 2.Rasterizer Start
    _rs = new Rasterizer;
  }
  ~PipeLine()
  {
    SAFE_RELEASE(_backBuffer);
    SAFE_RELEASE(_om);
    SAFE_RELEASE(_rs);
  }
};