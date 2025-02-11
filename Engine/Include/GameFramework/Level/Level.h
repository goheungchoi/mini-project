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
  
  Level(const std::string& name) : name{name} {}
  virtual ~Level() {}

  class World* GetWorld() { return world; }

  template <GameObjectType T>
  void AddGameObject(GameObject* object) { 
    gameObjects.push_back(object);
    gameObjectTypeMap.insert({std::type_index(typeid(T)), object});
  }

  const std::list<GameObject*>& GetGameObjectList() { return gameObjects; }

public:

  virtual void PrepareLevel() {}
  virtual void BeginLevel() {}

  virtual void DestroyLevel();
  virtual void CleanupLevel() {}
};

template <class T>
concept LevelType = std::is_base_of<Level, T>::value;
