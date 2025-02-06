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
    ComPtr<ID3D11DepthStencilState> depthstencilWriteState;
    ComPtr<ID3D11DepthStencilState> depthstencilReadState;
    ComPtr<ID3D11DepthStencilView> outLineStencilView;
  };

  struct Rasterizer
  {
    ComPtr<ID3D11RasterizerState> rasterizerState;
    D3D11_VIEWPORT viewPort;
  };
  Device* _device;

public:
  BackBuffer* _backBuffer = nullptr;
  OutputMerger* _om = nullptr;
  Rasterizer* _rsDefault = nullptr;
  Rasterizer* _rsWireFrame = nullptr;
  Rasterizer* _rsCullNone = nullptr;
  float _clearColor[4];
  float _clearColor2[4] = {0.2f, 0.2f, 0.2f, 1.f};

public:
  PipeLine(Device* device, SwapChain* swapChain, int width, int height)
      : _clearColor{0.f, 0.f, 0.f, 1.f}, _device{device}
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
    HR_T(_device->GetDevice()->CreateRenderTargetView(
        pBackBufferTexture.Get(), NULL, _backBuffer->mainRTV.GetAddressOf()));
    // main depth
    D3D11_TEXTURE2D_DESC depthTexdesc =
        CreateTexture2DDesc(width, height, DXGI_FORMAT_D24_UNORM_S8_UINT, 1,
                            D3D11_BIND_DEPTH_STENCIL, 1);
    ComPtr<ID3D11Texture2D> pDepthTexture = nullptr;
    HR_T(_device->GetDevice()->CreateTexture2D(&depthTexdesc, NULL,
                                               pDepthTexture.GetAddressOf()));
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format =
        DXGI_FORMAT_D24_UNORM_S8_UINT; // Depth 24-bit + Stencil 8-bit;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    HR_T(_device->GetDevice()->CreateDepthStencilView(
        pDepthTexture.Get(), &descDSV, _backBuffer->mainDSV.GetAddressOf()));
    // depth stencil test state
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = CreateDepthStencilDesc(
        true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, false);
    D3D11_DEPTH_STENCIL_DESC depthDisalbeStencilDesc =
        CreateDepthStencilDesc( // for defferd
            false, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, false);
    HR_T(_device->GetDevice()->CreateDepthStencilState(
        &depthStencilDesc, _om->depthStencilState.GetAddressOf()));
    HR_T(_device->GetDevice()->CreateDepthStencilState(
        &depthDisalbeStencilDesc,
        _om->depthDisableStencilState.GetAddressOf()));
    _device->GetImmContext()->OMSetDepthStencilState(
        _om->depthStencilState.Get(), // initial depth stencil state
        1);
    // blend state
    SetBlendOnEnable(true);
    // set main render target
    _device->GetImmContext()->OMSetRenderTargets(
        1, _backBuffer->mainRTV.GetAddressOf(), _backBuffer->mainDSV.Get());

    // ���ٽ� ���⸦ ���� ���� ����
    D3D11_DEPTH_STENCIL_DESC stencilDescWrite = {};
    stencilDescWrite.DepthEnable = FALSE; // ���� ���� ���
    stencilDescWrite.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    stencilDescWrite.DepthFunc = D3D11_COMPARISON_ALWAYS;

    stencilDescWrite.StencilEnable = TRUE; // ���ٽ� ���
    stencilDescWrite.StencilWriteMask = 0xFF; // ���ٽ��� ��� ��Ʈ ���� ����
    stencilDescWrite.StencilReadMask = 0xFF; // ���ٽ��� ��� ��Ʈ �б� ����

    // ���� ���̽������� ���ٽ� ����
    stencilDescWrite.FrontFace.StencilFailOp =
        D3D11_STENCIL_OP_KEEP; // ���ٽ� �׽�Ʈ ���� �� ����
    stencilDescWrite.FrontFace.StencilDepthFailOp =
        D3D11_STENCIL_OP_KEEP; // ���� �׽�Ʈ ���� �� ����
    stencilDescWrite.FrontFace.StencilPassOp =
        D3D11_STENCIL_OP_REPLACE; // �׽�Ʈ ��� �� ��ü
    stencilDescWrite.FrontFace.StencilFunc =
        D3D11_COMPARISON_ALWAYS; // �׻� ���
    stencilDescWrite.BackFace = stencilDescWrite.FrontFace;
    HR_T(_device->GetDevice()->CreateDepthStencilState(
        &stencilDescWrite, _om->depthstencilWriteState.GetAddressOf()));
    // ���ٽ� �б�(�׽�Ʈ)�� ���� ���� ����
    D3D11_DEPTH_STENCIL_DESC stencilDescRead = {};
    stencilDescRead.DepthEnable = FALSE;
    stencilDescRead.DepthWriteMask =
        D3D11_DEPTH_WRITE_MASK_ZERO; // ���� ���� ��Ȱ��ȭ
    stencilDescRead.DepthFunc = D3D11_COMPARISON_LESS;

    stencilDescRead.StencilEnable = TRUE;
    stencilDescRead.StencilReadMask = 0xFF;  // �б� ����ũ
    stencilDescRead.StencilWriteMask = 0x00; // ���� ��Ȱ��ȭ
    // ���� ���̽������� ���ٽ� ����
    stencilDescRead.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
    stencilDescRead.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; // ����
    stencilDescRead.FrontFace.StencilFailOp =
        D3D11_STENCIL_OP_KEEP; // ���� �� ����
    stencilDescRead.FrontFace.StencilDepthFailOp =
        D3D11_STENCIL_OP_KEEP; // ���� ���� �� ����
    stencilDescRead.BackFace = stencilDescRead.FrontFace;
    HR_T(_device->GetDevice()->CreateDepthStencilState(
        &stencilDescRead, _om->depthstencilReadState.GetAddressOf()));
    // Output Merger End
    //
    // 2.Rasterizer Start
    // defulat
    _rsDefault = new Rasterizer;
    _rsCullNone = new Rasterizer;
    _rsWireFrame = new Rasterizer;
    _rsDefault->viewPort = {.TopLeftX = 0.f,
                            .TopLeftY = 0.f,
                            .Width = static_cast<float>(width),
                            .Height = static_cast<float>(height),
                            .MinDepth = 0.f,
                            .MaxDepth = 1.f};
    D3D11_RASTERIZER_DESC rasterizerDesc =
        CreateRaterizerDesc(D3D11_FILL_SOLID, D3D11_CULL_BACK, false, 0, 0.f,
                            0.f, true, false, false, true);
    HR_T(_device->GetDevice()->CreateRasterizerState(
        &rasterizerDesc, _rsDefault->rasterizerState.GetAddressOf()));
    // wireFrame
    rasterizerDesc =
        CreateRaterizerDesc(D3D11_FILL_WIREFRAME, D3D11_CULL_NONE, false, 0,
                            0.f, 0.f, true, false, false, true);
    HR_T(_device->GetDevice()->CreateRasterizerState(
        &rasterizerDesc, _rsWireFrame->rasterizerState.GetAddressOf()));
    // cull none
    rasterizerDesc =
        CreateRaterizerDesc(D3D11_FILL_SOLID, D3D11_CULL_NONE, false, 0, 0.f,
                            0.f, true, false, false, true);
    HR_T(_device->GetDevice()->CreateRasterizerState(
        &rasterizerDesc, _rsCullNone->rasterizerState.GetAddressOf()));
    _device->GetImmContext()->RSSetState(_rsDefault->rasterizerState.Get());
    _device->GetImmContext()->RSSetViewports(1, &_rsDefault->viewPort);
    // Rasterizer End
  }
  ~PipeLine()
  {
    SAFE_RELEASE(_backBuffer);
    SAFE_RELEASE(_om);
    SAFE_RELEASE(_rsDefault);
  }

public:
  void ClearBackBufferRTV()
  {

    _device->GetImmContext()->ClearRenderTargetView(_backBuffer->mainRTV.Get(),
                                                    _clearColor2);
  }
  void ClearBackBufferDSV()
  {
    _device->GetImmContext()->ClearDepthStencilView(
        _backBuffer->mainDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.f, 0);
  }
  void SetBlendOnEnable(BOOL blendEnable)
  {
    // blend state
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = false; // ���� �� Ŀ������ ��Ȱ��ȭ
    blendDesc.IndependentBlendEnable = false; // ������ ���� ��Ȱ��ȭ
    D3D11_RENDER_TARGET_BLEND_DESC rtBlenddesc = CreateRTBlendDesc(blendEnable);
    blendDesc.RenderTarget[0] = rtBlenddesc; // ù��° rt�� state ����
    HR_T(_device->GetDevice()->CreateBlendState(
        &blendDesc, _om->blentState.GetAddressOf()));
    float blendFactor[4] = {0.f, 0.f, 0.f, 0.f};
    // �ʱ� blend���� true.
    _device->GetImmContext()->OMSetBlendState(_om->blentState.Get(),
                                              blendFactor, 0xFFFFFFFF);
  }
  void SetMainViewPort()
  {
    _device->GetImmContext()->RSSetViewports(1, &_rsDefault->viewPort);
  }
  void SetBackBuffer()
  {
    _device->GetImmContext()->OMSetRenderTargets(
        1, _backBuffer->mainRTV.GetAddressOf(), _backBuffer->mainDSV.Get());
  }
  void SetMainRS()
  {
    _device->GetImmContext()->RSSetState(_rsDefault->rasterizerState.Get());
  }
  void SetWireFrame()
  {
    _device->GetImmContext()->RSSetState(_rsWireFrame->rasterizerState.Get());
  }
  void SetCullNone()
  {
    _device->GetImmContext()->RSSetState(_rsCullNone->rasterizerState.Get());
  }
  D3D11_VIEWPORT GetMainViewPort() { return _rsDefault->viewPort; }
  void TurnZBufferOn()
  {
    _device->GetImmContext()->OMSetDepthStencilState(
        _om->depthStencilState.Get(), 1);
  }
  void TurnZBufferOff()
  {
    _device->GetImmContext()->OMSetDepthStencilState(
        _om->depthDisableStencilState.Get(), 1);
  }

  void SetStencilWrite()
  {
    _device->GetImmContext()->OMSetDepthStencilState(
        _om->depthstencilWriteState.Get(), 1);
  }
  void SetStencilRead()
  {
    _device->GetImmContext()->OMSetDepthStencilState(
        _om->depthstencilReadState.Get(), 1);
  }
  void ClearPixelShaderResourceView(int i)
  {
    ID3D11ShaderResourceView* null = nullptr;
    _device->GetImmContext()->PSSetShaderResources(i, 1, &null);
  }
};