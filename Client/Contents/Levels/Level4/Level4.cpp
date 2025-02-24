#include "Level4.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "Contents/UI/AudioDramaUI/AudioDramaUI.h"

void Level4::PrepareLevel()
{
  __super::PrepareLevel();
	mapWarehouseMeshHandle =
      LoadModel("Models\\Maps\\Map_003_Warehouse\\Map_003.glb");
}

void Level4::BeginLevel()
{
  __super::BeginLevel();

#ifdef USED2D
  inGameUI->_mainMission->SetText(L"협정 참가자 명단 확보");
  inGameUI->_subMission[0]->SetText(L"전투원 희생 없이 작전 수행");
  inGameUI->HideUI(L"SubMission_2");
  map->OnDeleteCharacter = [=]() { inGameUI->_agentStorage->ResetAgent(); };
  map->PauseGame();


  _audioDrama = world->_canvas->CreatePanel<AudioDramaUI>(L"AudioDramaUI");
  _audioDrama->stageidx = 4;
  inGameUI->Deactivate();

#endif
}

void Level4::CleanupLevel() {
  GameLevel::CleanupLevel();
  UnloadModel(mapWarehouseMeshHandle);
}

void Level4::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapWarehouseMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 2, kEast);
  map->CreateEnemyAt(kGunman, 1, 0, kNorth);
  map->CreateEnemyAt(kGunman, 3, 2, kWest);
  map->CreateEnemyAt(kGunman, 3, 5, kSouth);
  map->CreateEnemyAt(kGunman, 4, 2, kEast);
  map->CreateEnemyAt(kBrawler, 5, 0, kWest);
  map->CreateEnemyAt(kGunman, 5, 4, kEast);
  map->CreateEnemyAt(kBrawler, 5, 5, kSouth);

  map->CreateObstacleAt(ObstacleType_Drum, 0, 4, kNorth);
  map->CreateObstacleAt(ObstacleType_DrumOld, 0, 5, kNorth);
  map->CreateObstacleAt(ObstacleType_VBox, 4, 4, kNorth);
  map->CreateObstacleAt(ObstacleType_VBox, 4, 5, kNorth);
}

void Level4::TriggerAction() {}
