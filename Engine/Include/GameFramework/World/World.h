#pragma once

#include "GameFramework/Level/Level.h"

class World
{
protected:
  std::map<std::string, Level*> levelList;
  Level* nextLevel{nullptr};
  Level* currentLevel{nullptr};

  class CameraComponent* mainCamera{nullptr};

  // UI System
  // class Canvas* canvas = nullptr;

  // EffectSystem
  // EffectSystem effectSystem;

  // Collision System
  // CollisionSystem collisionSystem;

public:
  World() {}
  virtual ~World() {}

	template <LevelType T>
  T* CreateLevel(const std::string& name)
  {
    T* pLevel = new T(this, name);
    levelList.insert({name, pLevel});
    return pLevel;
  }

	void ChangeScene() { /* TODO: */ }
  void SetNextScene(std::string next) { /* TODO: */ }
  Level* GetCurrentLevel() { return currentLevel; }

};
