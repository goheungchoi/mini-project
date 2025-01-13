#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../SwapChain.h"
using namespace Microsoft::WRL;

struct BackBuffer
{
  ComPtr<ID3D11RenderTargetView> mainRTV;
  ComPtr<ID3D11DepthStencilView> mainDSV;
};
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

public:
  BackBuffer* _backBuffer = nullptr;
  OutputMerger* _om = nullptr;
  Rasterizer* _rs = nullptr;
  float _clearColor[4];
  float _clearColor2[4] = {0.2f, 0.2f, 0.2f,1.f};

public:
  PipeLine(Device* device, SwapChain* swapChain, int width, int height)
      : _clearColor{0.f,0.f,0.f,1.f}
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
                            D3D11_BIND_DEPTH_STENCIL,1);
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
    HR_T(device->GetDevice()->CreateDepthStencilState(
        &depthStencilDesc, _om->depthStencilState.GetAddressOf()));
    HR_T(device->GetDevice()->CreateDepthStencilState(
        &depthDisalbeStencilDesc,
        _om->depthDisableStencilState.GetAddressOf()));
    device->GetImmContext()->OMSetDepthStencilState(
        _om->depthStencilState.Get(), // initial depth stencil state
        1);
    // blend state
    SetBlendOnEnable(true, device);
    // set main render target
    device->GetImmContext()->OMSetRenderTargets(
        1, _backBuffer->mainRTV.GetAddressOf(), _backBuffer->mainDSV.Get());
    // Output Merger End
    //
    // 2.Rasterizer Start
    _rs = new Rasterizer;
    _rs->viewPort = {.TopLeftX = 0.f,
                     .TopLeftY = 0.f,
                     .Width = static_cast<float>(width),
                     .Height = static_cast<float>(height),
                     .MinDepth = 0.f,
                     .MaxDepth = 1.f};
    D3D11_RASTERIZER_DESC rasterizerDesc =
        CreateRaterizerDesc(D3D11_FILL_SOLID, D3D11_CULL_BACK, false, 0, 0.f,
                            0.f, true, false, false, true);
    HR_T(device->GetDevice()->CreateRasterizerState(
        &rasterizerDesc, _rs->rasterizerState.GetAddressOf()));
    device->GetImmContext()->RSSetState(_rs->rasterizerState.Get());
    device->GetImmContext()->RSSetViewports(1, &_rs->viewPort);
    // Rasterizer End
  }
  ~PipeLine()
  {
    SAFE_RELEASE(_backBuffer);
    SAFE_RELEASE(_om);
    SAFE_RELEASE(_rs);
  }

public:
  void ClearBackBuffer(Device* device)
  {
    
    device->GetImmContext()->ClearRenderTargetView(_backBuffer->mainRTV.Get(),
                                                   _clearColor2);
    device->GetImmContext()->ClearDepthStencilView(_backBuffer->mainDSV.Get(),
                                                   D3D11_CLEAR_DEPTH, 1.f, 0);
  }
  void SetBlendOnEnable(BOOL blendEnable, Device* device)
  {
    // blend state
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = false; // 알파 투 커버리지 비활성화
    blendDesc.IndependentBlendEnable = false; // 독립적 블렌드 비활성화
    D3D11_RENDER_TARGET_BLEND_DESC rtBlenddesc = CreateRTBlendDesc(blendEnable);
    blendDesc.RenderTarget[0] = rtBlenddesc; // 첫번째 rt에 state 설정
    HR_T(device->GetDevice()->CreateBlendState(&blendDesc,
                                               _om->blentState.GetAddressOf()));
    float blendFactor[4] = {0.f, 0.f, 0.f, 0.f};
    // 초기 blend상태 true.
    device->GetImmContext()->OMSetBlendState(_om->blentState.Get(), blendFactor,
                                             0xFFFFFFFF);
  }
  void SetBackBuffer(Device* device)
  {
    device->GetImmContext()->OMSetRenderTargets(
        1, _backBuffer->mainRTV.GetAddressOf(), _backBuffer->mainDSV.Get());
  }
  void TurnZBufferOn(Device* device)
  {
    device->GetImmContext()->OMSetDepthStencilState(
        _om->depthStencilState.Get(), 1);
  }
  void TurnZBufferOff(Device* device)
  {
    device->GetImmContext()->OMSetDepthStencilState(
        _om->depthDisableStencilState.Get(), 1);
  }
};