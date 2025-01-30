#pragma once
#include "Common.h"

class Device;
class SwapChain;
class Sprite;
class Font;
class Text;
class UIRenderer;
class Texture;

class D2DRenderer // D2D.ver
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

  void DrawRectangle(Color color, Vector4 rect, float stroke = 1.0f,
                     float opacity = 1.0f);
  void DrawEllipse(Color color, Vector2 ellipsePT, Vector2 radius,
                   float stroke = 1.0f, float opacity = 1.0f);

  void FillRectangle(Color color, Vector4 rect, float opacity = 1.0f);
  void FillEllipse(Color color, Vector2 ellipsePT, Vector2 radius,
                   float opacity = 1.0f);
  void DrawLine(Color color, Vector2 startPt, Vector2 endPt,
                float stroke = 1.0f, float opacity = 1.0f);

  void CreateSprite(LPCSTR path, Vector2 pos);
  void CreateText(const wchar_t* format, Vector4 rect,
                  const std::wstring& fontName, Color color);

private:
  void RenderSprites();
  void RenderTexts();

public:
  // Font
  Font* _pFont;

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

  std::unique_ptr<DirectX::SpriteBatch> _pSpriteBatch = nullptr;
};