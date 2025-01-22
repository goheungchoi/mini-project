#pragma once
#include "Common.h"

class Device;
class SwapChain;
class Font;
class D2DRenderer    // D2D.ver
{
public:
  D2DRenderer() = default;
  ~D2DRenderer();

public:
  bool Init(Device* device, SwapChain* swapChain);
  void CreateD2DRenderTarget();
  void UnInit();

  void Draw();
  void BeginDraw();
  void EndDraw();

  //ID2D1Bitmap1* ConvertDDSToD2DBitmap1(TextureData data);

public:
  Font* _pFont = nullptr;


private:
  Device* _pDevice = nullptr;
  SwapChain* _pSwapChain = nullptr;

  IDXGIDevice* _pDXGIDevice = nullptr;
  IDXGISurface* _pIDXGISurface = nullptr;

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
  void CreateTextFormat(
      std::wstring fontName, float size,
      UINT fontWeight = DWRITE_FONT_WEIGHT_REGULAR,
      UINT textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER,
      UINT paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

  void TextDraw(const wchar_t* format, Vector4 rect,
                const std::wstring& fontName = L"¸¼Àº °íµñ",
                Color color = Color(0.0f, 0.0f, 0.0f, 1.0f));

private:
  IDWriteTextFormat* FindFont(const std::wstring& fontName);


public:
  std::unordered_map<std::wstring, IDWriteTextFormat*> _TextFormats;

private:
  IDWriteFactory* pDWriteFactory = nullptr;
  ID2D1DeviceContext* _pD2D1DeviceContext = nullptr;
  ID2D1SolidColorBrush* _pBrush = nullptr;
};