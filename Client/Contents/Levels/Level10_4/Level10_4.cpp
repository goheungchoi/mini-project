#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "Level10_4.h"
void Level10_4::PrepareLevel()
{
  __super::PrepareLevel();
  mapBarMeshHandle = LoadModel("Models\\Maps\\Map_001_Bar\\Map_001.glb");
}

void Level10_4::BeginLevel()
{
  __super::BeginLevel();
#ifdef USED2D
  inGameUI->HideUI(L"SubMission");
  inGameUI->HideUI(L"SubMission_2");
  map->OnDeleteCharacter = [=]() { inGameUI->_agentStorage->ResetAgent(); };
  inGameUI->_mainMission->SetText(L"도전 모드 4");
#endif
}

void Level10_4::CleanupLevel()
{
  GameLevel::CleanupLevel();
  UnloadModel(mapBarMeshHandle);
}

void Level10_4::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapBarMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 2, kEast);
  map->CreateEnemyAt(kGunman, 0, 5, kEast);
  map->CreateEnemyAt(kBrawler, 1, 0, kWest);
  map->CreateEnemyAt(kGunman, 2, 0, kNorth);
  map->CreateEnemyAt(kGunman, 2, 4, kSouth);
  map->CreateEnemyAt(kGunman, 3, 2, kWest);
  map->CreateEnemyAt(kGunman, 4, 0, kNorth);
  map->CreateEnemyAt(kGunman, 4, 3, kSouth);
  map->CreateEnemyAt(kGunman, 5, 4, kWest);

  map->CreateCivillianAt( 3, 0, kNorth,false);
  map->CreateObstacleAt(ObstacleType_Box02, 0, 1, kNorth);
  map->CreateObstacleAt(ObstacleType_Box02, 0, 4, kEast);
  map->CreateObstacleAt(ObstacleType_Stool, 5, 3, kNorth);
}
