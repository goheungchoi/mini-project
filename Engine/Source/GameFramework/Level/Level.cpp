#include "GameFramework/Level/Level.h"

Level::Level(World* world, const std::string& name) : world{world}, name{name}
{
}

Level::~Level()
{
  for (auto& pGameObject : gameObjects)
  {
    delete pGameObject;
  }
  gameObjects.clear();
}

void Level::BeginPlay() {}

void Level::FixedUpdate(float _fixedRate) {}

void Level::Destroy() {}

void Level::CleanUp() {}

void Level::PreUpdate(float _dt) {}

void Level::Update(float _dt) {}

void Level::PostUpdate(float _dt) {}

void Level::Render() {}
