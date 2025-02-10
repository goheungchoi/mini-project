#include "Level3.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"

void Level3::PrepareLevel()
{
  __super::PrepareLevel();
}

void Level3::BeginLevel()
{
  __super::BeginLevel();

    #ifdef USED2D
  inGameUI->HideUI(L"GunfireBtn");
  inGameUI->HideUI(L"SubMission");
  inGameUI->HideUI(L"SubMission_2");

  inGameUI->_agentStorage->SetAgent(kBrawler, {AgentPos.x - spacing * 2, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kSlasher, {AgentPos.x - spacing * 1, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kGunman, {AgentPos.x, AgentPos.y});

  inGameUI->_mainMission->SetText(L"최대한 희생 없이 모든 적 처치");
#endif
}

void Level3::CleanupLevel() {}

void Level3::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapBarMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 3, kEast);
  map->CreateEnemyAt(kGunman, 1, 1, kNorth);
  map->CreateEnemyAt(kGunman, 3, 3, kWest);
  map->CreateEnemyAt(kBrawler, 4, 3, kSouth);
  map->CreateEnemyAt(kGunman, 4, 5, kSouth);
  map->CreateEnemyAt(kGunman, 5, 1, kEast);
  map->CreateEnemyAt(kBrawler, 5, 5, kWest);

  map->CreateObstacleAt(ObstacleType_Stool, 5, 3, kNorth);

}

void Level3::TriggerAction() {}
