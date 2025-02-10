#include "Level7.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "Contents/UI/ReconfirmUI/ReconfirmUI.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "Contents/UI/InGameUI/ApplyButton/ApplyButton.h"

void Level7::PrepareLevel() 
{
  __super::PrepareLevel();
	mapMuseumMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");
}

void Level7::BeginLevel()
{
  __super::BeginLevel();

#ifdef USED2D
  // world->_canvas->CreatePanel<ReconfirmUI>(L"TestReconfirmUI");

  inGameUI->_agentStorage->SetAgent(kBrawler, {AgentPos.x - spacing * 3, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kSlasher, {AgentPos.x - spacing * 2, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kGunman, {AgentPos.x - spacing, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kGunman, {AgentPos.x, AgentPos.y});

  inGameUI->_mainMission->SetText(L"�ִ��� ��� ���� ��� �� óġ");
  inGameUI->_subMission->SetText(L"�ΰ� ������ ���");
#endif

}

void Level7::CleanupLevel() {
  GameLevel::CleanupLevel();
	UnloadModel(mapMuseumMeshHandle);
}

void Level7::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMuseumMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 1, 0, kEast);
  map->CreateEnemyAt(kGunman, 2, 3, kSouth);
  map->CreateEnemyAt(kGunman, 2, 5, kEast);
  map->CreateEnemyAt(kGunman, 3, 0, kWest);
  map->CreateEnemyAt(kGunman, 3, 1, kNorth);
  map->CreateEnemyAt(kGunman, 3, 4, kSouth);
  map->CreateEnemyAt(kGunman, 4, 5, kSouth);
  map->CreateEnemyAt(kGunman, 5, 2, kNorth);
  map->CreateEnemyAt(kGunman, 5, 5, kSouth);

  map->CreateCivillianAt(3, 5, kWest, true);

  map->CreateObstacleAt(ObstacleType_Lion, 0, 0, kNorth);
  map->CreateObstacleAt(ObstacleType_Sofa, 2, 1, kSouth);
  map->CreateObstacleAt(ObstacleType_Sofa, 4, 1, kSouth);
  map->CreateObstacleAt(ObstacleType_Sofa, 5, 1, kSouth);
}

void Level7::TriggerAction()
{
}
