#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "Contents/GameObjects/Map/Map.h"

class GameManager : public GameObject
{

  Map* map;

public:
  static bool endingflag;
  GameManager(World* world);

  void Pause() { map->Deactivate(); }
  void Resume() { map->Activate(); }
  void GameTrigger() { map->TriggerAction(); }

  // Game loop events
  virtual void OnAwake();
  virtual void OnActivated() {}

  virtual void FixedUpdate(float fixedRate) {}
  virtual void PreUpdate(float dt) {}
  virtual void Update(float dt) {}
  virtual void PostUpdate(float dt) {}

private:
  void CreateInGameScene();
};