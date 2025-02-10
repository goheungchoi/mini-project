#include "Level4.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"


void Level4::PrepareLevel()
{
  __super::PrepareLevel();
	mapWarehouseMeshHandle =
      LoadModel("Models\\Maps\\Map_003_Warehouse\\Map_003.glb");
}

void Level4::BeginLevel()
{
  __super::BeginLevel();

#ifdef USED2D
  inGameUI->HideUI(L"GunfireBtn");
  inGameUI->HideUI(L"SubMission");

  inGameUI->_agentStorage->SetAgent(kBrawler, {AgentPos.x - spacing * 3, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kSlasher, {AgentPos.x - spacing * 2, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kGunman, {AgentPos.x - spacing, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kGunman, {AgentPos.x, AgentPos.y});

  inGameUI->_mainMission->SetText(L"최대한 희생 없이 모든 적 처치");
#endif
}

void Level4::CleanupLevel() {
  GameLevel::CleanupLevel();
  UnloadModel(mapWarehouseMeshHandle);
}

void Level4::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapWarehouseMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 2, kEast);
  map->CreateEnemyAt(kGunman, 1, 0, kNorth);
  map->CreateEnemyAt(kGunman, 3, 2, kWest);
  map->CreateEnemyAt(kGunman, 3, 5, kSouth);
  map->CreateEnemyAt(kGunman, 4, 2, kEast);
  map->CreateEnemyAt(kBrawler, 5, 0, kWest);
  map->CreateEnemyAt(kGunman, 5, 4, kEast);
  map->CreateEnemyAt(kBrawler, 5, 5, kSouth);

  map->CreateObstacleAt(ObstacleType_Drum, 0, 4, kNorth);
  map->CreateObstacleAt(ObstacleType_DrumOld, 0, 5, kNorth);
  map->CreateObstacleAt(ObstacleType_VBox, 4, 4, kNorth);
  map->CreateObstacleAt(ObstacleType_VBox, 4, 5, kNorth);
}

void Level4::TriggerAction() {}
