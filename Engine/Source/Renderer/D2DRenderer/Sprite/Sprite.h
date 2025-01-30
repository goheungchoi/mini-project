#pragma once
#include "Renderer/D2DRenderer/Common.h"

class Device;
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
  LPCSTR _path{};
  class Texture* _pTexture = nullptr;
  Vector2 _textureSize{};
  Vector2 _pos{};

  Device* _pDevice = nullptr;
};

class SpriteManager
{
private:
  SpriteManager() = default;
  ~SpriteManager() = default;

  SpriteManager(const SpriteManager&) = delete;
  SpriteManager& operator=(const SpriteManager&) = delete;
  SpriteManager(SpriteManager&&) noexcept = delete;
  SpriteManager& operator=(SpriteManager&&) noexcept = delete;

  static SpriteManager* m_pInstance;

public:
  static SpriteManager* GetInstance();
  std::shared_ptr<Sprite> GetSprite(LPCSTR path, Device* pDevice);
  void Destory();

public:
  std::unordered_map<std::string, std::shared_ptr<Sprite>> _spritePool;
};

#define _SpriteManager (*SpriteManager::GetInstance())
