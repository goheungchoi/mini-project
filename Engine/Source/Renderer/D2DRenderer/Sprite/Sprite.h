#pragma once
#include "Renderer/D2DRenderer/Common.h"

class Device;
class D2DRenderer;
class Sprite
{
public:
  Sprite(LPCSTR path);
  ~Sprite();

private:
  void Init(LPCSTR path);
  void UnInit();
  Vector2 CalculateTextureSize();

public:
  void Render();
  void Render(Vector4 rect);
  void RenderCursor();
  void RenderTransition();

  void SetPos(Vector2 pos) { _pos = pos; }
  void SetScale(Vector2 scale) { _scale = scale; }
  void SetOpacity(float opacity) { _opacity = opacity; }
  Vector2 GetTextureSize() { return _textureSize; }
  Vector2 GetPos() { return _pos; }

  static void SetDevice(Device* pDevice) { _pDevice = pDevice; }
  static void SetD2DRenderer(D2DRenderer* pD2DRenderer)
  {
    _pD2DRenderer = pD2DRenderer;
  }

private:
  static Device* _pDevice;
  static D2DRenderer* _pD2DRenderer;

  LPCSTR _path{};
  class Texture* _pTexture = nullptr;
  Vector2 _textureSize{};
  Vector2 _pos{};
  Vector2 _scale{1.0f, 1.0f};
  float _opacity = 1.f;
};