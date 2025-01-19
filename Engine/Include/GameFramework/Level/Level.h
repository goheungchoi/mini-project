#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "ResourceManager/ResourceManager.h"

class Level
{
  using GameObjectTypeMap =
      std::unordered_multimap<std::type_index, GameObject*>;

	std::string name;
  class World* world;

protected:
  GameObjectTypeMap gameObjectTypeMap;

public:
  Level(class World* world, const std::string& name);
  virtual ~Level();

	template <GameObjectType T>
  T* CreateGameObject()
  {
    T* newGameObject = new T(world);
    gameObjects.push_back(newGameObject);
    gameObjectTypeMap.insert({std::type_index(typeid(T)), newGameObject});
    return newGameObject;
  }

	template <GameObjectType T>
  T* CreateGameObjectFromModel(const char* path)
  {
    T* newGameObject = new T(world);
		// TODO:
    return newGameObject;


	
	}

public:
  std::list<GameObject*> gameObjects;

  class World* GetWorld() { return world; }

  virtual void Enter() { BeginPlay(); };
  virtual void Exit() = 0;
  virtual void Clear() = 0;

  virtual void BeginPlay();
  virtual void FixedUpdate(float _fixedRate);
  virtual void Destroy();
  virtual void CleanUp();
  virtual void PreUpdate(float _dt);
  virtual void Update(float _dt);
  virtual void PostUpdate(float _dt);
  virtual void Render();

};

template <class T>
concept LevelType = std::is_base_of<Level, T>::value;
