#include "Resource2DManager/Resource2DManager.h"
#include "Renderer/D2DRenderer/Sprite/Sprite.h"
#include "Renderer/D2DRenderer/Animation2D/Animation2D.h"

Resource2DManager* Resource2DManager::m_pInstance = nullptr;

Resource2DManager* Resource2DManager::GetInstance()
{
  if (m_pInstance == nullptr)
  {
    m_pInstance = new Resource2DManager();
  }

  return m_pInstance;
}

void Resource2DManager::Destroy()
{
  if (m_pInstance)
  {
    _Anim2DAssetMap.clear();
    _SpriteMap.clear();

    delete m_pInstance;
    m_pInstance = nullptr;
  }
}

bool Resource2DManager::LoadSprite(LPCSTR path)
{
  auto it = _SpriteMap.find(path);
  if (it == _SpriteMap.end())
  {
    auto newSprite = std::make_shared<Sprite>(path);
    _SpriteMap[path] = newSprite;
    return true;
  }
  else
  {
    return false;
  }
}

std::shared_ptr<Sprite> Resource2DManager::GetSprite(LPCSTR path)
{
  auto it = _SpriteMap.find(path);
  if (it != _SpriteMap.end())
  {
    return it->second;
  }
  else
  {
    throw std::runtime_error("Sprite is not found");
  }
}
