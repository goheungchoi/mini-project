#include "Contents/GameObjects/GameManager/GameManager.h"

#include "GameFramework/World/World.h"

GameManager::GameManager(World* world) : GameObject(world) {

}

void GameManager::OnAwake() {
  map = world->FindGameObjectByType<Map>();
  
  if (map==nullptr)
  {
    throw std::runtime_error("Map is not found");
  }

}

void GameManager::CreateInGameScene() {}
