#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "Level10_5.h"
void Level10_5::PrepareLevel()
{
  __super::PrepareLevel();
  mapMuseumMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");
}

void Level10_5::BeginLevel()
{
  __super::BeginLevel();
#ifdef USED2D
  inGameUI->HideUI(L"SubMission");
  inGameUI->HideUI(L"SubMission_2");
  map->OnDeleteCharacter = [=]() { inGameUI->_agentStorage->ResetAgent(); };
  inGameUI->_mainMission->SetText(L"도전 모드 5");
#endif
}

void Level10_5::CleanupLevel()
{
  GameLevel::CleanupLevel();
  UnloadModel(mapMuseumMeshHandle);
}

void Level10_5::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMuseumMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 0, kEast);
  map->CreateEnemyAt(kGunman, 0, 3, kSouth);
  map->CreateEnemyAt(kGunman, 0, 4, kNorth);
  map->CreateEnemyAt(kGunman, 2, 3, kSouth);
  map->CreateEnemyAt(kGunman, 3, 1, kNorth);
  map->CreateEnemyAt(kGunman, 3, 4, kSouth);
  map->CreateEnemyAt(kBrawler, 4, 5, kEast);
  map->CreateEnemyAt(kGunman, 5, 0, kWest);
  map->CreateEnemyAt(kGunman, 5, 3, kWest);

  map->CreateCivillianAt(2, 4, kWest,false);
  map->CreateObstacleAt(ObstacleType_Lion, 0, 1, kEast);
  map->CreateObstacleAt(ObstacleType_Sofa, 5, 2, kSouth);
}
