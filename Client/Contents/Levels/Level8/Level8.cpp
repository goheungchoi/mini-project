#include "Level8.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "Contents/UI/AudioDramaUI/AudioDramaUI.h"

void Level8::PrepareLevel()
{
  __super::PrepareLevel();
	mapMuseumMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");
}

void Level8::BeginLevel()
{
  __super::BeginLevel();

  #ifdef USED2D
  inGameUI->_mainMission->SetText(L"총리를 제거하여 협정 저지");
  inGameUI->_subMission[0]->SetText(L"전투원 희생 없이 작전 수행");
  map->OnDeleteCharacter = [=]() { inGameUI->_agentStorage->ResetAgent(); };
  _audioDrama = world->_canvas->CreatePanel<AudioDramaUI>(L"AudioDramaUI");
  _audioDrama->PlayFlag = false;
  _audioDrama->Deactivate();

#endif
}

void Level8::CleanupLevel() {
  GameLevel::CleanupLevel();
  UnloadModel(mapMuseumMeshHandle);
}

void Level8::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMuseumMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 1, kSouth);
  map->CreateEnemyAt(kGunman, 0, 3, kEast);
  map->CreateEnemyAt(kGunman, 0, 5, kEast);
  map->CreateEnemyAt(kGunman, 2, 0, kNorth);
  map->CreateEnemyAt(kGunman, 2, 4, kSouth);
  map->CreateEnemyAt(kGunman, 5, 0, kNorth);
  map->CreateEnemyAt(kBrawler, 5, 4, kSouth, true);
  map->CreateEnemyAt(kGunman, 5, 5, kWest);

  map->CreateObstacleAt(ObstacleType_Sofa, 1, 0, kSouth);
  map->CreateObstacleAt(ObstacleType_Sofa, 1, 4, kSouth);
  map->CreateObstacleAt(ObstacleType_Lion, 4, 0, kNorth);
}

void Level8::TriggerAction() {}
