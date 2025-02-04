#pragma once
#include "Renderer/D2DRenderer/Common.h"

class Device;
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
  void Render(DirectX::SpriteBatch* pSpriteBatch);
  void SetPos(Vector2 pos) { _pos = pos; }
  Vector2 GetTextureSize() { return _textureSize; }
  Vector2 GetPos() { return _pos; }

  static void SetDevice(Device* pDevice) { _pDevice = pDevice; }

private:
  static Device* _pDevice;

  LPCSTR _path{};
  class Texture* _pTexture = nullptr;
  Vector2 _textureSize{};
  Vector2 _pos{};

};