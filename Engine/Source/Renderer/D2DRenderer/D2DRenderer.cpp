#include "D2DRenderer.h"
#include "Renderer/DX11/Internal/Device.h"
#include "Renderer/DX11/Internal/SwapChain.h"
D2DRenderer::~D2DRenderer()
{
  UnInit();
}

bool D2DRenderer::Init(Device* device, SwapChain* swapChain)
{
  _pDevice = device;
  _pSwapChain = swapChain;

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
  HR_T(_pD2D1DeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),
                                              &_pBrush));

  CreateD2DRenderTarget();

  // Font 초기화
  _pFont = new Font(_pD2D1DeviceContext, _pBrush);

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
  HR_T(_pSwapChain->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&_pIDXGISurface)));

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

void D2DRenderer::Draw() {

}

void D2DRenderer::BeginDraw()
{
  _pD2D1DeviceContext->BeginDraw();
}

void D2DRenderer::EndDraw()
{
  _pD2D1DeviceContext->EndDraw();
}




/// <summary>
////////////////////// Font Engine //////////////////////////
/// </summary>

Font::Font(ID2D1DeviceContext* pD2D1DeviceContext, ID2D1SolidColorBrush* pBrush)
{
  _pD2D1DeviceContext = pD2D1DeviceContext;
  _pBrush = pBrush;

  Init();
}

Font::~Font()
{
  UnInit();
}

void Font::Init()
{
  CreateIDWriteFactory();
  CreateTextFormat(L"Agency FB", 100.0f);
  CreateTextFormat(L"궁서", 32.0f);
}

void Font::UnInit()
{
  for (auto& pair : _TextFormats)
  {
    if (pair.second)
    {
      Com::SAFE_RELEASE(pair.second);
    }
  }

  _TextFormats.clear();

  // IDWriteFactory 파괴
  Com::SAFE_RELEASE(pDWriteFactory);

  _pD2D1DeviceContext = nullptr;
  _pBrush = nullptr;
}

void Font::CreateIDWriteFactory()
{
  HR_T(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                           reinterpret_cast<IUnknown**>(&pDWriteFactory)));
}

void Font::CreateTextFormat(const std::wstring& fontName, float size, UINT fontWeight,
                            UINT textAlignment, UINT paragraphAlignment)
{
  IDWriteTextFormat* pTextFormat;

  //const wchar_t* formatName = L"맑은 고딕";

  HR_T(pDWriteFactory->CreateTextFormat(
      fontName.c_str(), // 글꼴 이름
      NULL,              // 글꼴 컬렉션 (NULL은 시스템 기본 사용)
      static_cast<DWRITE_FONT_WEIGHT>(fontWeight), DWRITE_FONT_STYLE_NORMAL,
      DWRITE_FONT_STRETCH_NORMAL,
      size, // 글꼴 크기
      L"ko-KR", // 로케일
      &pTextFormat));

  // 텍스트 정렬
  pTextFormat->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(textAlignment));
  pTextFormat->SetParagraphAlignment(
      static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(paragraphAlignment));

  _TextFormats.insert({fontName, pTextFormat});
}

void Font::TextDraw(const wchar_t* format, Vector4 rect,
                    const std::wstring& fontName, Color color)
{
  // 텍스트 그리기
                                //  left,    top,  right, bottom
  D2D1_RECT_F _rect = D2D1_RECT_F(rect.x, rect.y, rect.z, rect.w);

  D2D1_COLOR_F _color = D2D1::ColorF(color.x, color.y, color.z, color.w);

  _pBrush->SetColor(_color);

  IDWriteTextFormat* txtformat = FindFont(fontName);
  
  _pD2D1DeviceContext->DrawText(format, lstrlen(format) + 1, txtformat, _rect,
                                _pBrush);
}


IDWriteTextFormat* Font::FindFont(const std::wstring& fontName)
{
  auto iter = _TextFormats.find(fontName);

  if (iter != _TextFormats.end())
  {
    return iter->second;
  }

  return nullptr;
}
