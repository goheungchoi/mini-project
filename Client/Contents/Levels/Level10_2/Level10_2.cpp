#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "Level10_2.h"
void Level10_2::PrepareLevel()
{
  __super::PrepareLevel();
  mapWarehouseMeshHandle =
      LoadModel("Models\\Maps\\Map_003_Warehouse\\Map_003.glb");
}

void Level10_2::BeginLevel()
{
  __super::BeginLevel();
#ifdef USED2D
  inGameUI->HideUI(L"GunfireBtn");
  inGameUI->HideUI(L"SubMission");
  inGameUI->HideUI(L"SubMission_2");
  map->OnDeleteCharacter = [=]() { inGameUI->_agentStorage->ResetAgent(); };
  inGameUI->_mainMission->SetText(L"도전 모드 2");
#endif
}

void Level10_2::CleanupLevel()
{
  GameLevel::CleanupLevel();
  UnloadModel(mapWarehouseMeshHandle);
}

void Level10_2::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapWarehouseMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0,3, kEast);
  map->CreateEnemyAt(kBrawler, 0, 5, kEast);
  map->CreateEnemyAt(kGunman, 1, 1, kEast);
  map->CreateEnemyAt(kGunman, 2, 2, kNorth);
  map->CreateEnemyAt(kGunman, 2, 5, kSouth);
  map->CreateEnemyAt(kGunman, 4, 3, kSouth);
  map->CreateEnemyAt(kGunman, 5, 1, kWest);
  map->CreateEnemyAt(kGunman, 5, 3, kNorth);
  map->CreateObstacleAt(ObstacleType_Drum, 0, 0, kNorth);
  map->CreateObstacleAt(ObstacleType_DrumOld, 0, 2, kNorth);
  map->CreateObstacleAt(ObstacleType_Drum, 2, 0, kNorth);
  map->CreateObstacleAt(ObstacleType_Box02, 3, 3, kNorth);
  map->CreateObstacleAt(ObstacleType_VBox, 3, 5, kNorth);
  map->CreateObstacleAt(ObstacleType_VBox, 4, 5, kNorth);
}
