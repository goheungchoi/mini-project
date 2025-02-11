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
#include "Contents/UI/InGameUI/SubMission/SubMission.h"



void Level2::PrepareLevel()
{
  __super::PrepareLevel();
	mapMuseumMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");
}

void Level2::BeginLevel()
{
  __super::BeginLevel();
	
#ifdef USED2D
  inGameUI->HideUI(L"GunfireBtn");
  inGameUI->HideUI(L"SubMission_2");

  inGameUI->_agentStorage->SetAgent(kBrawler, {AgentPos.x - spacing * 2, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kSlasher, {AgentPos.x - spacing * 1, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kGunman, {AgentPos.x, AgentPos.y});

  inGameUI->_mainMission->SetText(L"회의장 내 칼트 병력 제거");
  inGameUI->_subMission[0]->SetText(L"전투원 희생 없이 작전 수행");
#endif
}

void Level2::CleanupLevel() {
  GameLevel::CleanupLevel();
  UnloadModel(mapMuseumMeshHandle);
}

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
