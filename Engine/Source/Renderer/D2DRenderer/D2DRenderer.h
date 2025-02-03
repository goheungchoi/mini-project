#pragma once
#include "Common.h"
#include "Font/Font.h"

class Device;
class SwapChain;
class Sprite;
class Text;
class UIRenderer;
class Texture;

class Render2DQueue
{
public:
  Render2DQueue() = default;
  ~Render2DQueue() = default;

public:
  void AddRender2DCmd(std::function<void()> command)
  {
    _Render2DCmds.push_back(command);
  }
  void ExecuteRender2DCmd()
  {
    for (auto& command : _Render2DCmds)
    {
      command();
    }

    _Render2DCmds.clear(); // 명령 실행 수 Queue 비우기
  }

private:
  std::vector<std::function<void()>> _Render2DCmds;
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
  void DrawTexts(const wchar_t* format, Vector4 rect,
                 Color color,
                 const TextFormatInfo& textFormatInfo);

private:
  void RenderSprites();

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

  Render2DQueue _render2DQueue;
};