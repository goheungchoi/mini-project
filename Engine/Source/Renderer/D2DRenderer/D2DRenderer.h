#pragma once
#include "Common.h"

class Device;
class SwapChain;
class Font;
class UIRenderer;

class D2DRenderer    // D2D.ver
{
public:
  D2DRenderer() = default;
  ~D2DRenderer();

public:
  bool Init(Device* device, SwapChain* swapChain, D3D11_VIEWPORT viewport);
  void CreateD2DRenderTarget();
  void UnInit();

  void Draw();
  void BeginDraw();
  void EndDraw();

  //ID2D1Bitmap1* ConvertDDSToD2DBitmap1(TextureData data);

public:
  Font* _pFont = nullptr;
  UIRenderer* _pUIRenderer = nullptr;

private:
  Device* _pDevice = nullptr;
  SwapChain* _pSwapChain = nullptr;
  D3D11_VIEWPORT _viewport{};

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
  void TextDraw(const wchar_t* format, Vector4 rect,
                const std::wstring& fontName = L"Agency FB",
                Color color = Color(1.0f, 0.0f, 1.0f, 1.0f));

private:
  void Init();
  void UnInit();

  void CreateIDWriteFactory();


private:
  void CreateTextFormat(
      const std::wstring& fontName, float size,
      UINT fontWeight = DWRITE_FONT_WEIGHT_REGULAR,
      UINT textAlignment = DWRITE_TEXT_ALIGNMENT_CENTER,
      UINT paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

  IDWriteTextFormat* FindFont(const std::wstring& fontName);


public:
  std::unordered_map<std::wstring, IDWriteTextFormat*> _TextFormats;

private:
  ID2D1DeviceContext* _pD2D1DeviceContext = nullptr;
  ID2D1SolidColorBrush* _pBrush = nullptr;
  IDWriteFactory* pDWriteFactory = nullptr;
};



class UIRenderer
{
public:
  UIRenderer(ID3D11DeviceContext* pD3D1DeviceContext, D3D11_VIEWPORT viewport);
  ~UIRenderer();

private:
  void Init();
  void UnInit();

  private:
  ID3D11DeviceContext* _pD3D1DeviceContext = nullptr;
  D3D11_VIEWPORT _viewport{};

  std::unique_ptr<DirectX::SpriteBatch> spriteBatch = nullptr;
};