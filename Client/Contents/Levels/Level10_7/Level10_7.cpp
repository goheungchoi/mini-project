#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "Level10_7.h"
void Level10_7::PrepareLevel()
{
  __super::PrepareLevel();
  mapWarehouseMeshHandle = LoadModel("Models\\Maps\\Map_003_Warehouse\\Map_003.glb");
}

void Level10_7::BeginLevel()
{
  __super::BeginLevel();
#ifdef USED2D
  inGameUI->HideUI(L"GunfireBtn");
  inGameUI->HideUI(L"SubMission");
  inGameUI->HideUI(L"SubMission_2");
  map->OnDeleteCharacter = [=]() { inGameUI->_agentStorage->ResetAgent(); };
  inGameUI->_mainMission->SetText(L"모든 적 처치");
#endif
}

void Level10_7::CleanupLevel()
{
  GameLevel::CleanupLevel();
  UnloadModel(mapWarehouseMeshHandle);
}

void Level10_7::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapWarehouseMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 2, kSouth);
  map->CreateEnemyAt(kGunman, 0, 3, kEast);
  map->CreateEnemyAt(kGunman, 2, 0, kNorth);
  map->CreateEnemyAt(kGunman, 2, 5, kSouth);
  map->CreateEnemyAt(kGunman, 3, 2, kWest);
  map->CreateEnemyAt(kGunman, 4, 0, kWest);
  map->CreateEnemyAt(kGunman, 4, 3, kWest);
  map->CreateEnemyAt(kGunman, 5, 2, kWest);
  map->CreateEnemyAt(kGunman, 5, 4, kWest);

  map->CreateCivillianAt(5, 3, kWest, false);
  map->CreateObstacleAt(ObstacleType_DrumOld, 0, 5, kNorth);
  map->CreateObstacleAt(ObstacleType_Drum, 1, 0, kNorth);
  map->CreateObstacleAt(ObstacleType_VBox, 4, 5, kNorth);
}
