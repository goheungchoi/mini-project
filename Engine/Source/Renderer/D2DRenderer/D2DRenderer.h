#pragma once
#include "Common.h"

class Device;
class SwapChain;
class Font;
class Text;
class UIRenderer;
class Texture;

class Sprite
{
public:
  Sprite(LPCSTR path, Device* pDevice);
  ~Sprite();

private:
  void Init(LPCSTR path);
  void UnInit();
  Vector2 CalculateTextureSize();

  public:
  void Render(DirectX::SpriteBatch* pSpriteBatch);
  void SetPos(Vector2 pos) { _pos = pos; }

private:
  Texture* _pTexture = nullptr;
  Vector2 _textureSize{};
  Vector2 _pos{};

  Device* _pDevice = nullptr;

};

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

  void CreateSprite(LPCSTR path, Vector2 pos);
  void DrawSprites();

  void AddText(const wchar_t* format, Vector4 rect,
               const std::wstring& fontName, Color color);
  void DeleteText();

private:
  void RenderText();

public:
  // Font
  Font* _pFont;
  std::vector<Text*> _TextList;

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
  std::vector<Sprite*> _Sprites;
};