#pragma once
#include "Common.h"

class Device;
class SwapChain;
class Font;
class D2DRenderer    // D2D.ver
{
public:
  D2DRenderer() = default;
  ~D2DRenderer() = default;

public:
  bool Init();
  void CreateD2DRenderTarget();

  void Draw();
  void BeginDraw();
  void EndDraw();

  void SetDevice(Device* device) { _pDevice = device; }
  void SetSwapChain(SwapChain* swapChain) { _pSwapChain = swapChain; }

  ID2D1Bitmap1* ConvertDDSToD2DBitmap1(TextureData data);

public:
  Font* _pFont;


private:
  Device* _pDevice = nullptr;
  SwapChain* _pSwapChain = nullptr;

  IDXGIDevice* _pDXGIDevice = nullptr;
  IDXGISurface* _pIDXGISurface =
      nullptr; // Direct3D와 Direct2D 간의 데이터 교환을 가능하게 해줌

  ID2D1Device* _pD2D1Device = nullptr;
  ID2D1Factory1* _pD2DFactory = nullptr;
  ID2D1DeviceContext* _pD2D1DeviceContext = nullptr;
  ID2D1SolidColorBrush* _pBrush = nullptr;
  ID2D1Bitmap1* _pID2D1Bitmap = nullptr;

};


class Font
{
public:
  Font(ID2D1DeviceContext* pD2D1DeviceContext, ID2D1SolidColorBrush* pBrush);
  ~Font();

public:
  void Init();
  void UnInit();

  void CreateIDWriteFactory();
  void CreateTextFormat(std::wstring _fontName, float _size);
  void TextDraw(const wchar_t* format, D2D1_RECT_F _rect,
                D2D1_COLOR_F _color = D2D1::ColorF(D2D1::ColorF::Black), ...);


public:
  std::unordered_map<std::wstring, IDWriteTextFormat*> _TextFormats;

private:
  IDWriteFactory* pDWriteFactory = nullptr;
  ID2D1DeviceContext* _pD2D1DeviceContext = nullptr;
  ID2D1SolidColorBrush* _pBrush = nullptr;
};