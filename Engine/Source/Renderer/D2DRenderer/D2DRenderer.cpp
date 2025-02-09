﻿#include "D2DRenderer.h"
#include "Renderer/DX11/Internal/Device.h"
#include "Renderer/DX11/Internal/SwapChain.h"
#include "Resource2DManager/Resource2DManager.h"
#include "Sprite/Sprite.h"
#include "Shared/Config/Config.h"

D2DRenderer::~D2DRenderer()
{
  UnInit();
}

bool D2DRenderer::Init(Device* device, SwapChain* swapChain,
                       D3D11_VIEWPORT viewport)
{
  _pDevice = device;
  _pSwapChain = swapChain;
  _viewport = viewport;

  // IDXGIDevice 생성
  HR_T(_pDevice->GetDevice()->QueryInterface(__uuidof(IDXGIDevice),
                                             (void**)&_pDXGIDevice));

  // Direct2D 팩토리 생성
  HR_T(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_pD2DFactory));

  // DXGI 디바이스를 사용하여 Direct2D 디바이스를 생성
  HR_T(_pD2DFactory->CreateDevice(_pDXGIDevice, &_pD2D1Device));

  // ID2D1DeviceContext 생성
  HR_T(_pD2D1Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
                                         &_pD2D1DeviceContext));

  // brush 생성
  HR_T(_pD2D1DeviceContext->CreateSolidColorBrush(
      D2D1::ColorF(D2D1::ColorF::Black), &_pBrush));

  CreateD2DRenderTarget();

  // SpriteBatch 생성 및 설정
  _pSpriteBatch =
      std::make_unique<DirectX::SpriteBatch>(_pDevice->GetImmContext());
  _pSpriteBatch->SetViewport(_viewport);

  // Font 초기화
  fs::path fontDir = Utility::convertToUTF16(ns::kFontDir);

  _pFont = new Font;
  _pFont->LoadFontFile((fontDir / "PT Noeul.ttf").wstring(),
                       L"PT Noeul");

  Sprite::SetDevice(device);
  Sprite::SetD2DRenderer(this);

  return true;
}

void D2DRenderer::CreateD2DRenderTarget()
{
  // 현재 창의 DPI(1인치당 픽셀의 개수) 설정 가져오기
  float dpiX, dpiY;
  UINT dpi = GetDpiForWindow(_pSwapChain->GetWindowHandle());
  dpiX = static_cast<float>(dpi);
  dpiY = static_cast<float>(dpi);

  // DXGI 표면 가져오기
  // Microsoft::WRL::ComPtr<IDXGISurface> dxgiSurface;
  HR_T(
      _pSwapChain->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&_pIDXGISurface)));

  // Direct2D 비트맵 속성 정의
  D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
      D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
      D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
      dpiX,
      dpiY); // DXGI_FORMAT_UNKNOWN을 사용하면 Direct2D가 적합한 포맷을 자동으로
             // 선택함.

  // DXGI 표면을 기반으로 Direct2D 비트맵 생성
  HR_T(_pD2D1DeviceContext->CreateBitmapFromDxgiSurface(
      _pIDXGISurface, &bitmapProperties, &_pID2D1Bitmap));

  // 비트맵을 DeviceContext의 렌더 타겟으로 설정
  _pD2D1DeviceContext->SetTarget(_pID2D1Bitmap);
}

void D2DRenderer::UnInit()
{
  Resource2DManager::GetInstance()->Destroy();
  // SpriteBatch의 해제
  if (_pSpriteBatch)
  {
    _pSpriteBatch.reset();
  }
  SAFE_RELEASE(_pFont);
  Com::SAFE_RELEASE(_pBrush);
  Com::SAFE_RELEASE(_pID2D1Bitmap);
  Com::SAFE_RELEASE(_pD2D1DeviceContext);
  Com::SAFE_RELEASE(_pD2DFactory);
  Com::SAFE_RELEASE(_pD2D1Device);
  Com::SAFE_RELEASE(_pIDXGISurface);
  Com::SAFE_RELEASE(_pDXGIDevice);

  _pDevice = nullptr;
  _pSwapChain = nullptr;
}

void D2DRenderer::Draw() {}

void D2DRenderer::BeginDraw() {}

void D2DRenderer::EndDraw()
{
  // Sprite Render
  BeginSprites();
  _d2dRenderQueue.ExecuteSpriteRenderCmd();
  EndSprites();

  // 2D Geometry 및 text Render
  _pD2D1DeviceContext->BeginDraw();
  _d2dRenderQueue.Execute2DRenderCmd();
  _pD2D1DeviceContext->EndDraw();

  BeginSprites();
  _d2dRenderQueue.ExecuteLateSpriteRenderCmd();
  _d2dRenderQueue.ExcuteTransitionRenderCmd();
  _d2dRenderQueue.ExcuteCursorRenderCmd();
  EndSprites();



}

void D2DRenderer::DrawRectangle(Color color, Vector4 rect, float stroke,
                                float opacity)
{
  _d2dRenderQueue.AddRender2DCmd([=]() {
    D2D1_COLOR_F clr = D2D1::ColorF(color.x, color.y, color.z, color.w);
    D2D1_RECT_F rt = D2D1_RECT_F(rect.x, rect.y, rect.z, rect.w);
    //  left,    top,  right, bottom

    _pBrush->SetColor(clr);
    _pBrush->SetOpacity(opacity);
    _pD2D1DeviceContext->DrawRectangle(rt, _pBrush, stroke);
  });
}

void D2DRenderer::DrawEllipse(Color color, Vector2 ellipsePT, Vector2 radius,
                              float stroke, float opacity)
{
  D2D1_COLOR_F clr = D2D1::ColorF(color.x, color.y, color.z, color.w);
  D2D1_ELLIPSE ellipse = {{ellipsePT.x, ellipsePT.y}, radius.x, radius.y};

  _pBrush->SetColor(clr);
  _pBrush->SetOpacity(opacity);
  _pD2D1DeviceContext->DrawEllipse(ellipse, _pBrush, stroke);
}

void D2DRenderer::FillRectangle(Color color, Vector4 rect, float opacity)
{
  D2D1_COLOR_F clr = D2D1::ColorF(color.x, color.y, color.z, color.w);
  D2D1_RECT_F rt = D2D1_RECT_F(rect.x, rect.y, rect.z, rect.w);

  _pBrush->SetColor(clr);
  _pBrush->SetOpacity(opacity);
  _pD2D1DeviceContext->FillRectangle(rt, _pBrush);
}

void D2DRenderer::FillEllipse(Color color, Vector2 ellipsePT, Vector2 radius,
                              float opacity)
{
  D2D1_COLOR_F clr = D2D1::ColorF(color.x, color.y, color.z, color.w);
  D2D1_ELLIPSE ellipse = {{ellipsePT.x, ellipsePT.y}, radius.x, radius.y};

  _pBrush->SetColor(clr);
  _pBrush->SetOpacity(opacity);
  _pD2D1DeviceContext->FillEllipse(ellipse, _pBrush);
}

void D2DRenderer::DrawLine(Color color, Vector2 startPt, Vector2 endPt,
                           float stroke, float opacity)
{
  D2D1_COLOR_F clr = D2D1::ColorF(color.x, color.y, color.z, color.w);
  D2D1_POINT_2F _startPt = {startPt.x, startPt.y};
  D2D1_POINT_2F _endPt = {endPt.x, endPt.y};

  _pBrush->SetColor(clr);
  _pBrush->SetOpacity(opacity);
  _pD2D1DeviceContext->DrawLine(_startPt, _endPt, _pBrush, stroke);
}

void D2DRenderer::CreateSprite(LPCSTR path, Vector2 pos)
{
  // IMG 객체를 생성하고 반환
  auto newSprite = Resource2DManager::GetInstance()->GetSprite(path);
  newSprite->SetPos(pos);
}

void D2DRenderer::DrawTexts( std::wstring format, Vector4 rect, Color color,
                            float opacity, const TextFormatInfo& textFormatInfo)
{
  _d2dRenderQueue.AddRender2DCmd([=]() {
    IDWriteTextFormat* textFormat = nullptr;
    bool fontValid = true;

    // 1. 커스텀 폰트 컬렉션 획득
    IDWriteFontCollection1* pCustomCollection =
        _pFont->GetFontCollection(textFormatInfo._fontName);

    // 2. 컬렉션 존재 여부 확인
    if (!pCustomCollection)
    {
      OutputDebugString(L"[ERROR] Font collection not found: ");
      OutputDebugString(textFormatInfo._fontName.c_str());
      OutputDebugString(L"\n");
      fontValid = false;
    }
    else
    {
      // 3. 실제 패밀리 이름 존재 여부 확인
      UINT32 index = 0;
      BOOL exists = FALSE;
      HRESULT hr = pCustomCollection->FindFamilyName(
          textFormatInfo._fontName.c_str(), &index, &exists);

      if (FAILED(hr) || !exists)
      {
        OutputDebugString(L"[ERROR] Font family not in collection: ");
        OutputDebugString(textFormatInfo._fontName.c_str());
        OutputDebugString(L"\n");
        fontValid = false;
      }
    }

    // 4. 폰트가 유효하지 않으면 시스템 폰트 사용 & 경고
    if (!fontValid)
    {
      MessageBoxW(nullptr, L"커스텀 폰트 로드 실패! 시스템 폰트로 대체됩니다.",
                  L"Font Error", MB_ICONWARNING);

      // 시스템 폰트 컬렉션으로 대체
      pCustomCollection = nullptr;
    }

    // 렌더링 전 캐시 검색
    FontKey key = {textFormatInfo._fontName,
                static_cast<DWRITE_FONT_WEIGHT>(textFormatInfo._fontWeight),
                static_cast<DWRITE_FONT_STYLE>(textFormatInfo._fontStyle),
                static_cast<DWRITE_FONT_STRETCH>(textFormatInfo._fontStretch),
                textFormatInfo._fontSize};
        
    if (textFormatCache.find(key) != textFormatCache.end())
    {
      textFormat = textFormatCache[key].Get();
    }
    else
    {
      // 새로 생성 후 캐시에 저장
      HR_T(_pFont->GetIDWriteFactory()->CreateTextFormat(
          textFormatInfo._fontName.c_str(), pCustomCollection,
          static_cast<DWRITE_FONT_WEIGHT>(textFormatInfo._fontWeight),
          static_cast<DWRITE_FONT_STYLE>(textFormatInfo._fontStyle),
          static_cast<DWRITE_FONT_STRETCH>(textFormatInfo._fontStretch),
          textFormatInfo._fontSize, // 글꼴 크기
          L"ko-KR",                 // 로케일
          &textFormat));
      textFormatCache.emplace(key, textFormat);
    }


    // 텍스트 정렬
    textFormat->SetTextAlignment(
        static_cast<DWRITE_TEXT_ALIGNMENT>(textFormatInfo._textAlignment));
    textFormat->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(
        textFormatInfo._paragraphAlignment));

    textFormat->SetLineSpacing(
        DWRITE_LINE_SPACING_METHOD_UNIFORM, // 일정한 간격 적용
        60.0f, // 줄 높이 (기본 폰트 크기보다 크면 간격 증가)
        30.0f  // 기준선 오프셋 (보통 0 또는 폰트 크기의 일부)
    );

    // 텍스트 그리기
    D2D1_RECT_F rec = D2D1_RECT_F(rect.x, rect.y, rect.z, rect.w);

    D2D1_COLOR_F clr = D2D1::ColorF(color.x, color.y, color.z, color.w);

    _pBrush->SetColor(clr);
    _pBrush->SetOpacity(opacity);

    _pD2D1DeviceContext->DrawText(format.c_str(), lstrlen(format.c_str()) + 1, textFormat, rec,
                                  _pBrush);
  });
}

void D2DRenderer::BeginSprites()
{
  _pDevice->GetImmContext()->OMGetDepthStencilState(&_prevDepthState,
                                                    &_stencilRef);

  float blendFactor[4] = {0, 0, 0, 0};
  UINT* psamplemask = new UINT[1];
  _pDevice->GetImmContext()->OMGetBlendState(&_blendState, blendFactor,
                                             psamplemask);

  _pSpriteBatch->Begin(DirectX::DX11::SpriteSortMode_Deferred, _blendState);
}

void D2DRenderer::EndSprites()
{
  _pSpriteBatch->End();

  // DepthStencilState 복원
  _pDevice->GetImmContext()->OMSetDepthStencilState(_prevDepthState,
                                                    _stencilRef);
}
