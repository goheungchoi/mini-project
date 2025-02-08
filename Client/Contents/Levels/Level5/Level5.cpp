#include "Level5.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"

void Level5::PrepareLevel()
{
  __super::PrepareLevel();
}

void Level5::BeginLevel()
{
  __super::BeginLevel();

  #ifdef USED2D
  inGameUI->_agentStorage->SetAgent(kBrawler, {1200, 960});
  inGameUI->_agentStorage->SetAgent(kSlasher, {1400, 960});
  inGameUI->_agentStorage->SetAgent(kGunman, {1600, 960});
  inGameUI->_agentStorage->SetAgent(kGunman, {1800, 960});
  inGameUI->_mainMission->SetText(L"(선택) 최대한 희생 없이 모든 적 처치");
  inGameUI->_subMission->SetText(L"(선택) 시작의 총성으로 적 처치");
#endif
}

void Level5::CleanupLevel() {}

void Level5::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMuseumMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 3, kEast);
  map->CreateEnemyAt(kBrawler, 0, 5, kSouth);
  map->CreateEnemyAt(kGunman, 1, 5, kWest);
  map->CreateEnemyAt(kGunman, 2, 1, kNorth);
  map->CreateEnemyAt(kGunman, 2, 5, kSouth);
  map->CreateEnemyAt(kGunman, 4, 3, kWest);
  map->CreateEnemyAt(kGunman, 5, 1, kWest);
  map->CreateEnemyAt(kGunman, 5, 4, kSouth);
  map->CreateEnemyAt(kBrawler, 5, 5, kWest);

  map->CreateObstacleAt(ObstacleType_Lion, 4, 5, kSouth);
  map->CreateObstacleAt(ObstacleType_Sofa, 5, 3, kWest);

}

void Level5::TriggerAction() {}
