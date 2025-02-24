﻿#pragma once
#include "Common.h"
#include "Font/Font.h"

class Device;
class SwapChain;
class Sprite;
class Text;
class UIRenderer;
class Texture;

// 렌더링 패스 타입
enum class PassType2D
{
  SPRITE_BATCH,
  DEFAULT
};

class Render2DQueue
{
public:
  Render2DQueue() = default;
  ~Render2DQueue() = default;

public:
  void AddRender2DCmd(std::function<void()> command,
                      PassType2D type = PassType2D::DEFAULT)
  {
    if (type == PassType2D::DEFAULT)
    {
      _2DRenderCmds.push_back(command);
    }
    else if (type == PassType2D::SPRITE_BATCH)
    {
      _SpriteRenderCmds.push_back(command);
    }
  }
  void AddLateRender2DCmd(std::function<void()> command)
  {
    _SpriteLateRenderCmds.push_back(command);
  }


  void Execute2DRenderCmd()
  {
    for (auto& command : _2DRenderCmds)
    {
      command();
    }

    _2DRenderCmds.clear(); // 명령 실행 수 Queue 비우기
  }

  void ExecuteSpriteRenderCmd()
  {
    for (auto& command : _SpriteRenderCmds)
    {
      command();
    }

    _SpriteRenderCmds.clear();
  }
  void ExecuteLateSpriteRenderCmd()
  {
    for (auto& command : _SpriteLateRenderCmds)
    {
      command();
    }

    _SpriteLateRenderCmds.clear();
  }
  void SetCursorRenderCmd(std::function<void()> event)
  {
    _cursorRenderCmd = event;
  }
  void SetTransitionRenderCmd(std::function<void()> event)
  {
    _transitionRenderCmd = event;
  }
  void ExcuteCursorRenderCmd() { _cursorRenderCmd(); }
  void ExcuteTransitionRenderCmd()
  {
    _transitionRenderCmd();
  }



private:
  std::vector<std::function<void()>> _2DRenderCmds;
  std::vector<std::function<void()>> _SpriteRenderCmds;
  std::vector<std::function<void()>> _SpriteLateRenderCmds;
  std::function<void()> _transitionRenderCmd = [] () {}; 
  std::function<void()> _cursorRenderCmd = []() {}; 

};

struct FontKey
{
  std::wstring familyName;
  DWRITE_FONT_WEIGHT weight;
  DWRITE_FONT_STYLE style;
  DWRITE_FONT_STRETCH stretch;
  float size;

  // 비교 연산자 오버로딩 (필수)
  bool operator==(const FontKey& other) const
  {
    return familyName == other.familyName && weight == other.weight &&
           style == other.style && stretch == other.stretch &&
           size == other.size;
  }
};

namespace std
{
template <>
struct hash<FontKey>
{
  size_t operator()(const FontKey& key) const
  {
    return hash<wstring>()(key.familyName) ^
           (hash<int>()(static_cast<int>(key.weight)) << 1) ^
           (hash<int>()(static_cast<int>(key.style)) << 2) ^
           (hash<int>()(static_cast<int>(key.stretch)) << 3) ^
           (hash<float>()(key.size) << 4);
  }
};
} // namespace std

class D2DRenderer // D2D.ver
{
  friend class Sprite;

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
  void DrawTexts(std::wstring format, Vector4 rect, Color color,
                 float opacity, const TextFormatInfo& textFormatInfo);



private:
  void BeginSprites();
  void EndSprites();

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
  ID3D11DepthStencilState* _prevDepthState = nullptr;
  ID3D11BlendState* _blendState = nullptr;

  UINT _stencilRef{};

  Render2DQueue _d2dRenderQueue;
  std::unordered_map<FontKey, Microsoft::WRL::ComPtr<IDWriteTextFormat>> textFormatCache;


};