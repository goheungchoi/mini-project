#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "Level10_3.h"
void Level10_3::PrepareLevel()
{
  __super::PrepareLevel();
  mapMuseumMeshHandle =
      LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");
}

void Level10_3::BeginLevel()
{
  __super::BeginLevel();
#ifdef USED2D
  //inGameUI->HideUI(L"GunfireBtn");
  inGameUI->HideUI(L"SubMission");
  inGameUI->HideUI(L"SubMission_2");
  map->OnDeleteCharacter = [=]() { inGameUI->_agentStorage->ResetAgent(); };
  inGameUI->_mainMission->SetText(L"모든 적 처치");
#endif
}

void Level10_3::CleanupLevel()
{
  GameLevel::CleanupLevel();
  UnloadModel(mapMuseumMeshHandle);
}

void Level10_3::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMuseumMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 1, kEast);
  map->CreateEnemyAt(kGunman, 1, 0, kNorth);
  map->CreateEnemyAt(kGunman, 1, 3, kEast);
  map->CreateEnemyAt(kGunman, 1, 5, kEast);
  map->CreateEnemyAt(kGunman, 4, 1, kWest);
  map->CreateEnemyAt(kGunman, 4, 5, kWest);
  map->CreateEnemyAt(kGunman, 5, 0, kNorth);
  map->CreateEnemyAt(kGunman, 5, 3, kWest);
  map->CreateEnemyAt(kBrawler, 5, 5, kSouth);

  map->CreateObstacleAt(ObstacleType_Lion, 0, 5, kSouth);
}
