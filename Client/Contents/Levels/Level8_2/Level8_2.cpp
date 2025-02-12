#include "Level8_2.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"

#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "Contents/UI/AudioDramaUI/AudioDramaUI.h"


void Level8_2::PrepareLevel()
{
  __super::PrepareLevel();
	mapMuseumMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");
}

void Level8_2::BeginLevel()
{
  __super::BeginLevel();

#ifdef USED2D
  inGameUI->_mainMission->SetText(L"�Ѹ��� �����Ͽ� ���� ����");
  inGameUI->_subMission[0]->SetText(L"������ ��� ���� ���� ����");
  inGameUI->_subMission[1]->SetText(L"�ΰ� ��� ���� ���� ����");
  map->OnDeleteCharacter = [=]() { inGameUI->_agentStorage->ResetAgent(); };

  _audioDrama = world->_canvas->CreatePanel<AudioDramaUI>(L"AudioDramaUI");
  _audioDrama->PlayFlag = false;

  _audioDrama->SetTotalTime(25.f);

#endif
}

void Level8_2::CleanupLevel() {
  GameLevel::CleanupLevel();
  UnloadModel(mapMuseumMeshHandle);
}

void Level8_2::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMuseumMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 0, kEast);
  map->CreateEnemyAt(kGunman, 0, 2, kNorth);
  map->CreateEnemyAt(kGunman, 0, 5, kSouth);
  map->CreateEnemyAt(kGunman, 1, 1, kEast);
  map->CreateEnemyAt(kGunman, 1, 5, kSouth);
  map->CreateEnemyAt(kGunman, 4, 0, kWest);
  map->CreateEnemyAt(kBrawler, 4, 3, kWest, true);
  map->CreateEnemyAt(kGunman, 5, 1, kNorth);
  map->CreateEnemyAt(kGunman, 5, 5, kSouth);

  map->CreateCivillianAt(2, 5, kSouth);

  map->CreateObstacleAt(ObstacleType_Lion, 3, 5, kSouth);
  map->CreateObstacleAt(ObstacleType_Lion, 4, 4, kWest);
}

void Level8_2::TriggerAction() {}
