#include "Level3.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

void Level3::PrepareLevel()
{
  __super::PrepareLevel();
}

void Level3::BeginLevel()
{
  __super::BeginLevel();
}

void Level3::CleanupLevel() {}

void Level3::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMeshHandle);
  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);
}

void Level3::TriggerAction() {}
