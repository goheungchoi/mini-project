#include "Level7.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "Contents/UI/ReconfirmUI/ReconfirmUI.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "Contents/UI/InGameUI/ApplyButton/ApplyButton.h"
#include "Contents/UI/AudioDramaUI/AudioDramaUI.h"
void Level7::PrepareLevel() 
{
  __super::PrepareLevel();
	mapMuseumMeshHandle = LoadModel("Models\\Maps\\Map_002_Museum\\Map_002.glb");
}

void Level7::BeginLevel()
{
  __super::BeginLevel();

#ifdef USED2D
  // world->_canvas->CreatePanel<ReconfirmUI>(L"TestReconfirmUI");

  inGameUI->_mainMission->SetText(L"연설장 잠입 및 초대장 확보");
  inGameUI->_subMission[0]->SetText(L"전투원 희생 없이 작전 수행");
  inGameUI->_subMission[1]->SetText(L"부관 희생 없이 작전 수행");
  map->OnDeleteCharacter = [=]() { inGameUI->_agentStorage->ResetAgent(); };

  inGameUI->_subMission[0]->SetOpacity(0.f);

  _audioDrama = world->_canvas->CreatePanel<AudioDramaUI>(L"AudioDramaUI");
  _audioDrama->SetTotalTime(25.f);
#endif

}

void Level7::CleanupLevel() {
  GameLevel::CleanupLevel();
	UnloadModel(mapMuseumMeshHandle);
}

void Level7::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapMuseumMeshHandle);

  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kGunman, 0, 3, kEast);
  map->CreateEnemyAt(kGunman, 2, 1, kWest);
  map->CreateEnemyAt(kGunman, 2, 2, kNorth);
  map->CreateEnemyAt(kGunman, 2, 4, kSouth);
  map->CreateEnemyAt(kGunman, 4, 1, kNorth);
  map->CreateEnemyAt(kGunman, 4, 4, kSouth);
  map->CreateEnemyAt(kGunman, 5, 3, kWest);


  map->CreateCivillianAt(2, 5, kSouth, true);

  map->CreateObstacleAt(ObstacleType_Sofa, 4, 5, kEast);
  map->CreateObstacleAt(ObstacleType_Sofa, 5, 5, kEast);
}

void Level7::TriggerAction()
{
}
