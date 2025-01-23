#pragma once

#include "ResourceManager/ResourceManager.h"
#include "GameFramework/GameObject/GameObject.h"

class Level
{
  using GameObjectTagMap = std::unordered_multimap<std::string, GameObject*>;
  using GameObjectTypeMap = std::unordered_multimap<std::type_index, GameObject*>;
  using GameObjectNameMap = std::unordered_multimap<std::string, GameObject*>;

public:
  std::string name;
  class World* world{nullptr};

  GameObjectTagMap gameObjectTagMap;
  GameObjectTypeMap gameObjectTypeMap;
  GameObjectNameMap gameObjectNameMap;
  std::list<GameObject*> gameObjects;

  Level(const std::string& name);
  virtual ~Level() {}

  class World* GetWorld() { return world; }

  template <GameObjectType T>
  void AddGameObject(GameObject* object) { 
    gameObjects.push_back(object); 
    gameObjectTypeMap.insert({std::type_index(typeid(T)), object});
  }

  template<GameObjectType T>
  T* FindGameObjectByType()
  {
    auto it = gameObjectTypeMap.find(std::type_index(typeid(T)));
    if (it == gameObjectTypeMap.end())
      return nullptr;
    return static_cast<T*>(it->second);
  }

  template<GameObjectType T>
  std::vector<T*> FindAllGameObjectByType()
  {
    std::vector<T*> res;
    auto range = gameObjectTypeMap.equal_range(std::type_index(typeid(T)));
    for (auto it = range.first; it != range.second; ++it)
    {
      res.push_back(static_cast<T*>(it->second));
    }
    return res;
  }
  
  GameObject* GetGameObjectByTag(const std::string& tag) {
    auto it = gameObjectTagMap.find(tag);
    if (it == gameObjectTagMap.end())
      return nullptr;
    return it->second;
  }

  std::vector<GameObject*> FindAllGameObjectsByTag(const std::string& tag)
  {
    std::vector<GameObject*> res;
    auto range = gameObjectTagMap.equal_range(tag);
    for (auto it = range.first; it != range.second; ++it)
    {
      res.push_back(it->second);
    }
    return res;
  }

  const std::list<GameObject*>& GetGameObjectList() { return gameObjects; }

public:

  virtual void PrepareLevel() {}
  virtual void BeginLevel() {}

  virtual void DestroyLevel() {}
  virtual void CleanupLevel() {}
};

template <class T>
concept LevelType = std::is_base_of<Level, T>::value;
