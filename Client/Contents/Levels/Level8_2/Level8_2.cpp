#include "Level8_2.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
//#include "Contents/UI/InGameUI/SubMission/SubMission.h"

void Level8_2::PrepareLevel()
{
  __super::PrepareLevel();
	mapMuseumMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");
}

void Level8_2::BeginLevel()
{
  __super::BeginLevel();

#ifdef USED2D
  inGameUI->HideUI(L"SubMission");

  inGameUI->_agentStorage->SetAgent(kBrawler, {AgentPos.x - spacing * 3, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kSlasher, {AgentPos.x - spacing * 2, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kGunman, {AgentPos.x - spacing, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kGunman, {AgentPos.x, AgentPos.y});

  inGameUI->_mainMission->SetText(L"최대한 희생 없이 모든 적 처치");
#endif
}

void Level8_2::CleanupLevel() {
  GameLevel::CleanupLevel();
  UnloadModel(mapMuseumMeshHandle);
}

void Level8_2::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMuseumMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 0, kEast);
  map->CreateEnemyAt(kGunman, 0, 2, kNorth);
  map->CreateEnemyAt(kGunman, 0, 5, kSouth);
  map->CreateEnemyAt(kGunman, 1, 1, kEast);
  map->CreateEnemyAt(kGunman, 1, 5, kSouth);
  map->CreateEnemyAt(kGunman, 4, 0, kWest);
  map->CreateEnemyAt(kGunman, 4, 3, kWest);
  map->CreateEnemyAt(kGunman, 5, 1, kNorth);
  map->CreateEnemyAt(kGunman, 5, 5, kSouth);

  map->CreateCivillianAt(2, 5, kSouth);

  map->CreateObstacleAt(ObstacleType_Lion, 3, 5, kSouth);
  map->CreateObstacleAt(ObstacleType_Lion, 4, 4, kWest);
}

void Level8_2::TriggerAction() {}
