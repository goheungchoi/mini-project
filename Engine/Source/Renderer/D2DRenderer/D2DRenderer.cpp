#include "D2DRenderer.h"
#include "Renderer/DX11/Internal/Device.h"
#include "Renderer/DX11/Internal/SwapChain.h"
#include "Font/Font.h"
#include "Sprite/Sprite.h"

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
  HR_T(_pD2D1DeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black),
                                              &_pBrush));

  CreateD2DRenderTarget();

  // SpriteBatch 생성 및 설정
  _pSpriteBatch =
      std::make_unique<DirectX::SpriteBatch>(_pDevice->GetImmContext());
  _pSpriteBatch->SetViewport(_viewport);


  // Font 초기화
  _pFont = new Font;


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
  _SpriteManager.Destory();
  // SpriteBatch의 해제
  if (_pSpriteBatch) { _pSpriteBatch.reset(); }
  DeleteTextAll();
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
  RenderText();
  _pD2D1DeviceContext->EndDraw();
}

void D2DRenderer::CreateSprite(LPCSTR path, Vector2 pos)
{
  // IMG 객체를 생성하고 반환
  auto newSprite = _SpriteManager.GetSprite(path, _pDevice);
  newSprite->SetPos(pos);
}

void D2DRenderer::DrawSprites()
{
  // 현재 DepthStencilState 저장
  ID3D11DepthStencilState* prevDepthState = nullptr;
  UINT stencilRef;

  _pDevice->GetImmContext()->OMGetDepthStencilState(&prevDepthState,
                                                    &stencilRef);

  _pSpriteBatch->Begin();

  // 모든 Sprite Render
  if (!_SpriteManager._spritePool.empty())
  {
    for (auto sprite : _SpriteManager._spritePool)
    {
      sprite.second->Render(_pSpriteBatch.get());
    }
  }

  _pSpriteBatch->End();

  // DepthStencilState 복원
  _pDevice->GetImmContext()->OMSetDepthStencilState(prevDepthState, stencilRef);
}


void D2DRenderer::AddText(const wchar_t* format, Vector4 rect,
                          const std::wstring& fontName, Color color)
{
  Text* newText = new Text(format, rect, fontName, color);

  _TextList.push_back(newText);
}

void D2DRenderer::DeleteText(const wchar_t* format)
{
  //auto it = std::remove_if(_TextList.begin(), _TextList.end(),
  //                         [format](const Text* text) {
  //                           return wcscmp(text->_format.c_str(), format) == 0;
  //                         });

  //for (auto i = it; i != _TextList.end(); ++i)
  //{
  //  delete *i;
  //}

  //_TextList.erase(it, _TextList.end());
}

void D2DRenderer::DeleteTextAll()
{
  if (!_TextList.empty())
  {
    for (auto txt : _TextList)
    {
      SAFE_RELEASE(txt);
    }
  }
}

void D2DRenderer::RenderText()
{
  if (_TextList.empty())
  {
    return;
  }

  for (auto txt : _TextList)
  {
    // 텍스트 그리기
    //  left,    top,  right, bottom
    D2D1_RECT_F rect =
        D2D1_RECT_F(txt->_rect.x, txt->_rect.y, txt->_rect.z, txt->_rect.w);

    D2D1_COLOR_F color = D2D1::ColorF(txt->_color.x, txt->_color.y,
                                      txt->_color.z, txt->_color.w);

    _pBrush->SetColor(color);

    IDWriteTextFormat* txtformat = _pFont->FindFont(txt->_fontName);

    _pD2D1DeviceContext->DrawText(txt->_format.c_str(),
                                  lstrlen(txt->_format.c_str()) + 1,
                                  txtformat,
                                  rect,
                                  _pBrush);
  }

}
