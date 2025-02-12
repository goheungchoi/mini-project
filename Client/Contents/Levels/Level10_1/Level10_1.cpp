#include "Level10_1.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "GameFramework/UI/Canvas/Canvas.h"
void Level10_1::PrepareLevel()
{
  __super::PrepareLevel();
  mapBarMeshHandle = LoadModel("Models\\Maps\\Map_001_Bar\\Map_001.glb");
}

void Level10_1::BeginLevel()
{
  __super::BeginLevel();
#ifdef USED2D
  inGameUI->HideUI(L"SubMission");
  inGameUI->HideUI(L"SubMission_2");
  map->OnDeleteCharacter = [=]() { inGameUI->_agentStorage->ResetAgent(); };
  inGameUI->_mainMission->SetText(L"도전 모드 1");
#endif
}

void Level10_1::CleanupLevel()
{
  GameLevel::CleanupLevel();
  UnloadModel(mapBarMeshHandle);
}

void Level10_1::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapBarMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 5, kSouth);
  map->CreateEnemyAt(kGunman, 1, 0, kNorth);
  map->CreateEnemyAt(kGunman, 1, 4, kEast);
  map->CreateCivillianAt(2, 2, kWest, false);
  map->CreateEnemyAt(kGunman, 3, 2, kNorth);
  map->CreateEnemyAt(kGunman, 3, 5, kWest);
  map->CreateEnemyAt(kGunman, 4, 0, kNorth);
  map->CreateEnemyAt(kGunman, 5, 2, kWest);
  map->CreateObstacleAt(ObstacleType_Stool, 5, 3, kNorth);
  map->CreateObstacleAt(ObstacleType_Stool, 5, 5, kNorth);
}
