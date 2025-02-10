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
}

void Level8::BeginLevel()
{
  __super::BeginLevel();

  #ifdef USED2D
  inGameUI->HideUI(L"GunfireBtn");
  inGameUI->HideUI(L"SubMission");

  inGameUI->_agentStorage->SetAgent(kBrawler, {AgentPos.x - spacing * 3, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kSlasher, {AgentPos.x - spacing * 2, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kGunman, {AgentPos.x - spacing, AgentPos.y});
  inGameUI->_agentStorage->SetAgent(kGunman, {AgentPos.x, AgentPos.y});

  inGameUI->_mainMission->SetText(L"최대한 희생 없이 모든 적 처치");

    _audioDrama = world->_canvas->CreatePanel<AudioDramaUI>(L"AudioDramaUI");
  _audioDrama->SetTotalTime(25.f);
      _audioDrama->Deactivate();

#endif
}

void Level8::CleanupLevel() {}

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
  map->CreateEnemyAt(kBrawler, 5, 4, kSouth);
  map->CreateEnemyAt(kGunman, 5, 5, kWest);

  map->CreateObstacleAt(ObstacleType_Sofa, 1, 0, kNorth);
  map->CreateObstacleAt(ObstacleType_Sofa, 1, 4, kSouth);
  map->CreateObstacleAt(ObstacleType_Lion, 4, 0, kNorth);
}

void Level8::TriggerAction() {}
