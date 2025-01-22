#include "D2DRenderer.h"
#include "../DX11/Internal/Device.h"
#include "../DX11/Internal/SwapChain.h"

bool D2DRenderer::Init()
{
  // IDXGIDevice ����
  HR_T(_pDevice->GetDevice()->QueryInterface(__uuidof(IDXGIDevice),
                                             (void**)&_pDXGIDevice));

  // Direct2D ���丮 ����
  HR_T(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_pD2DFactory));

  // DXGI ����̽��� ����Ͽ� Direct2D ����̽��� ����
  HR_T(_pD2DFactory->CreateDevice(_pDXGIDevice, &_pD2D1Device));

  // ID2D1DeviceContext ����
  HR_T(_pD2D1Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
                                          &_pD2D1DeviceContext));

  // brush ����
  HR_T(_pD2D1DeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),
                                              &_pBrush));

  CreateD2DRenderTarget();

  // Font �ʱ�ȭ
  _pFont = new Font(_pD2D1DeviceContext, _pBrush);

  return true;
}

void D2DRenderer::CreateD2DRenderTarget()
{
  // ���� â�� DPI(1��ġ�� �ȼ��� ����) ���� ��������
  float dpiX, dpiY;
  UINT dpi = GetDpiForWindow(_pSwapChain->GetWindowHandle());
  dpiX = static_cast<float>(dpi);
  dpiY = static_cast<float>(dpi);

  // DXGI ǥ�� ��������
  // Microsoft::WRL::ComPtr<IDXGISurface> dxgiSurface;
  HR_T(_pSwapChain->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&_pIDXGISurface)));

  // Direct2D ��Ʈ�� �Ӽ� ����
  D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
      D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
      D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
      dpiX,
      dpiY); // DXGI_FORMAT_UNKNOWN�� ����ϸ� Direct2D�� ������ ������ �ڵ�����
             // ������.

  // DXGI ǥ���� ������� Direct2D ��Ʈ�� ����
  HR_T(_pD2D1DeviceContext->CreateBitmapFromDxgiSurface(
      _pIDXGISurface, &bitmapProperties, &_pID2D1Bitmap));

  // ��Ʈ���� DeviceContext�� ���� Ÿ������ ����
  _pD2D1DeviceContext->SetTarget(_pID2D1Bitmap);
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

ID2D1Bitmap1* D2DRenderer::ConvertDDSToD2DBitmap1(TextureData data)
{
  return nullptr;
}





/// <summary>
////////////////////// Font Engine //////////////////////////
/// </summary>



Font::Font(ID2D1DeviceContext* pD2D1DeviceContext, ID2D1SolidColorBrush* pBrush)
{
  Init();
}

Font::~Font() {


}

void Font::Init()
{
  CreateIDWriteFactory();
  CreateTextFormat(L"Cooper", 32.0f);
}

void Font::UnInit()
{

}

void Font::CreateIDWriteFactory()
{
  HR_T(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                           reinterpret_cast<IUnknown**>(&pDWriteFactory)));
}

void Font::CreateTextFormat(std::wstring _fontName, float _size)
{
  IDWriteTextFormat* pTextFormat;

  HR_T(pDWriteFactory->CreateTextFormat(
      _fontName.c_str(), // �۲� �̸�
      NULL,      // �۲� �÷��� (NULL�� �ý��� �⺻ ���)
      DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
      DWRITE_FONT_STRETCH_NORMAL,
      _size, // �۲� ũ��
      L"",   // ������
      &pTextFormat));

  // �ؽ�Ʈ�� ���� �� �������� �߾ӿ� ����ϴ�.
  pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
  pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

  _TextFormats.insert({_fontName, pTextFormat});
}

void Font::TextDraw(const wchar_t* format, D2D1_RECT_F _rect,
                    D2D1_COLOR_F _color, ...)
{
  // �ؽ�Ʈ �׸���
  _pBrush->SetColor(_color);

  auto txtformat = _TextFormats[0];

  _pD2D1DeviceContext->DrawText(format, // �ؽ�Ʈ ����
                                lstrlen(format) + 1, txtformat, _rect, _pBrush);
}
