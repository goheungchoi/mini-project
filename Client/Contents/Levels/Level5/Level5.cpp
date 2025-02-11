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
	mapMuseumMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");
}

void Level5::BeginLevel()
{
  __super::BeginLevel();

  #ifdef USED2D
  inGameUI->HideUI(L"SubMission_2");

  inGameUI->_agentStorage->SetAgent(kBrawler, {AgentPos.x - spacing * 3, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kSlasher, {AgentPos.x - spacing * 2, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kGunman, {AgentPos.x - spacing, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kGunman, {AgentPos.x, AgentPos.y});

  inGameUI->_mainMission->SetText(L"협정 일정 정보 확보");
  inGameUI->_subMission[0]->SetText(L"전투원 희생 없이 작전 수행");
#endif
}

void Level5::CleanupLevel() {
  GameLevel::CleanupLevel();
  UnloadModel(mapMuseumMeshHandle);
}

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
