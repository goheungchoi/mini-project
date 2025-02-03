#pragma once
#include "Core/Common.h"

class Sprite;
class Anim2DAsset;
class Resource2DManager
{
private:
  Resource2DManager() = default;
  ~Resource2DManager() = default;

  Resource2DManager(const Resource2DManager&) = delete;
  Resource2DManager& operator=(const Resource2DManager&) = delete;
  Resource2DManager(Resource2DManager&&) noexcept = delete;
  Resource2DManager& operator=(Resource2DManager&&) noexcept = delete;

  static Resource2DManager* m_pInstance;

public:
  static Resource2DManager* GetInstance();
  void Destroy();

  //bool LoadSprite(LPCSTR path);
  std::shared_ptr<Sprite> GetSprite(LPCSTR path);

  //bool LoadAnim2DAsset(LPCSTR spritePath, LPCSTR animCSV);
  //std::shared_ptr<Anim2DAsset> GetAnim2DAsset(std::wstring animAssetName);

//private:
  std::map<std::string, std::shared_ptr<Sprite>> _SpriteMap;
  std::map<std::string, std::shared_ptr<Anim2DAsset>> _Anim2DAssetMap;
};
