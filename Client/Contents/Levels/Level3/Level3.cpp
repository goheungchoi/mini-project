#include "Level3.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"

void Level3::PrepareLevel()
{
  __super::PrepareLevel();
	mapBarMeshHandle = LoadModel("Models\\Maps\\Map_001_Bar\\Map_001.glb");
}

void Level3::BeginLevel()
{
  __super::BeginLevel();

    #ifdef USED2D
  inGameUI->HideUI(L"GunfireBtn");
  inGameUI->HideUI(L"SubMission_2");
  map->OnDeleteCharacter = [=]() { inGameUI->_agentStorage->ResetAgent(); };

  inGameUI->_mainMission->SetText(L"선술집 내 칼트 병력 제거");
  inGameUI->_subMission[0]->SetText(L"전투원 희생 없이 작전 수행");
#endif
}

void Level3::CleanupLevel()
{
  GameLevel::CleanupLevel();
  UnloadModel(mapBarMeshHandle);
}

void Level3::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapBarMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 3, kEast);
  map->CreateEnemyAt(kGunman, 1, 1, kNorth);
  map->CreateEnemyAt(kGunman, 3, 3, kWest);
  map->CreateEnemyAt(kBrawler, 4, 3, kSouth);
  map->CreateEnemyAt(kGunman, 4, 5, kSouth);
  map->CreateEnemyAt(kGunman, 5, 1, kWest);
  map->CreateEnemyAt(kBrawler, 5, 5, kWest);

  map->CreateCivillianAt(1, 5, kSouth, false);

  map->CreateObstacleAt(ObstacleType_Stool, 5, 3, kNorth);

}

void Level3::TriggerAction() {}
