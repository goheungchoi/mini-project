#include "Level6.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"

void Level6::PrepareLevel()
{
  __super::PrepareLevel();
}

void Level6::BeginLevel()
{
  __super::BeginLevel();

#ifdef USED2D
  inGameUI->HideUI(L"SubMission");
  inGameUI->_agentStorage->SetAgent(kBrawler, {1200, 960});
  inGameUI->_agentStorage->SetAgent(kSlasher, {1400, 960});
  inGameUI->_agentStorage->SetAgent(kGunman, {1600, 960});
  inGameUI->_agentStorage->SetAgent(kGunman, {1800, 960});
  inGameUI->_mainMission->SetText(L"최대한 희생 없이 모든 적 처치");
#endif
}

void Level6::CleanupLevel() {}

void Level6::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapWarehouseMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 0, kNorth);
  map->CreateEnemyAt(kGunman, 0, 2, kEast);
  map->CreateEnemyAt(kGunman, 1, 0, kNorth);
  map->CreateEnemyAt(kGunman, 1, 3, kEast);
  map->CreateEnemyAt(kBrawler, 1, 5, kWest);
  map->CreateEnemyAt(kGunman, 3, 5, kSouth);
  map->CreateEnemyAt(kGunman, 4, 0, kEast);
  map->CreateEnemyAt(kGunman, 5, 3, kWest);
  map->CreateEnemyAt(kGunman, 5, 4, kNorth);

  map->CreateObstacleAt(ObstacleType_VBox, 2, 0, kNorth);
  map->CreateObstacleAt(ObstacleType_DrumOld, 2, 5, kNorth);
  map->CreateObstacleAt(ObstacleType_Drum, 4, 1, kNorth);
  map->CreateObstacleAt(ObstacleType_DrumOld, 4, 5, kNorth);
  map->CreateObstacleAt(ObstacleType_Drum, 5, 1, kNorth);
}

void Level6::TriggerAction() {}
