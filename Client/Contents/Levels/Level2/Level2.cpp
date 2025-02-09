#include "Level2.h"

#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/UI/DialogUI/DialogUI.h"
#include "Contents/UI/TransitionUI/TransitionUI.h"
#include "Resource2DManager/Resource2DManager.h"

#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"



void Level2::PrepareLevel()
{
  __super::PrepareLevel();
}

void Level2::BeginLevel()
{
  __super::BeginLevel();

#ifdef USED2D
  inGameUI->HideUI(L"GunfireBtn");
  inGameUI->HideUI(L"SubMission");

  inGameUI->_agentStorage->SetAgent(kBrawler, {1400, 960});
  inGameUI->_agentStorage->SetAgent(kSlasher, {1600, 960});
  inGameUI->_agentStorage->SetAgent(kGunman, {1800, 960});
  inGameUI->_mainMission->SetText(L"최대한 희생 없이 모든 적 처치");
#endif
}

void Level2::CleanupLevel() {}

void Level2::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMuseumMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 0, kEast);
  map->CreateEnemyAt(kGunman, 0, 5, kEast);
  map->CreateEnemyAt(kBrawler, 2, 3, kNorth);
  map->CreateEnemyAt(kGunman, 4, 5, kWest);
  map->CreateEnemyAt(kBrawler, 5, 2, kWest);
  map->CreateEnemyAt(kGunman, 5, 3, kEast);


  map->CreateObstacleAt(ObstacleType_Lion, 0, 1, kSouth);
  map->CreateObstacleAt(ObstacleType_Sofa, 0, 4, kNorth);
  map->CreateObstacleAt(ObstacleType_Sofa, 1, 5, kWest);
  map->CreateObstacleAt(ObstacleType_Sofa, 4, 4, kNorth);
  map->CreateObstacleAt(ObstacleType_Lion, 5, 1, kNorth);
  map->CreateObstacleAt(ObstacleType_Sofa, 5, 5, kWest);
}

void Level2::TriggerAction() {}
