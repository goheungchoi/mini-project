#pragma once
#include "Common.h"

class Device;
class SwapChain;
class Font;
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

  void CreateSprite(LPCSTR path);
  void DrawSprites();

public:
  Font* _pFont = nullptr;

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

  // 컴퓨터에 설치되어있는 글꼴 말고 폴더에 있는 글꼴 가져다 쓰는 코드 추가 해야함
};



