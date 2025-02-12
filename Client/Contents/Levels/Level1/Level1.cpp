#include "Level1.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Resource2DManager/Resource2DManager.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/AudioDramaUI/AudioDramaUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"
#include "Contents/GameObjects/Map/Characters/Character.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "Contents/UI/TutorialUI/TutorialUI.h"

// TODO: Sound

void Level1::PrepareLevel()
{
  __super::PrepareLevel();

	mapBarMeshHandle = LoadModel("Models\\Maps\\Map_001_Bar\\Map_001.glb");

    // UI Resource Load
#ifdef USED2D
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Tuto\\PopupUI_01.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Tuto\\PopupUI_02.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Tuto\\PopupUI_03.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Tuto\\PopupUI_04.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Tuto\\PopupButton_default.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Tuto\\PopupButton_hover.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Tuto\\PopupButton2_default.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Tuto\\PopupButton2_hover.png");
#endif // USED2D

}

void Level1::BeginLevel()
{
  __super::BeginLevel();

  #ifdef USED2D

  inGameUI->HideUI(L"GunfireBtn");
  inGameUI->HideUI(L"SubMission_2");

  map->OnDeleteCharacter = [=]() {
        inGameUI->_agentStorage->ResetAgent();
      };

  map->PauseGame();

  inGameUI->_mainMission->SetText(L"선술집 내 칼트 병력 제거");
  inGameUI->_subMission[0]->SetText(L"전투원 희생 없이 작전 수행");

  _audioDrama = world->_canvas->CreatePanel<AudioDramaUI>(L"AudioDramaUI");
  _audioDrama->SetTotalTime(25.f);

  _tutorialUI = world->_canvas->CreatePanel<TutorialUI>(L"TutorialUI");
  _tutorialUI->Deactivate();

  // Audio Drama 나오고 나서 Tutorial 나오고 게임 시작 해야함
  #endif
}

void Level1::DestroyLevel()
{
  Level::DestroyLevel();
}

void Level1::CleanupLevel() {
  GameLevel::CleanupLevel();
  UnloadModel(mapBarMeshHandle);
}

void Level1::CreateMap()
{
  pivot = world->CreateGameObject();

  map = world->CreateGameObjectFromModel<Map>(mapBarMeshHandle);
 
  pivot->AddChildGameObject(map);
  map->Translate(-4, 0, -4);

  map->CreateEnemyAt(kBrawler, 2, 3, kSouth);
  map->CreateEnemyAt(kGunman, 3, 1, kWest);
  map->CreateEnemyAt(kGunman, 5, 3, kEast);
  map->CreateEnemyAt(kGunman, 5, 4, kEast);

  map->CreateObstacleAt(ObstacleType_Stool, 4, 3, kNorth);
  map->CreateObstacleAt(ObstacleType_Stool, 4, 4, kNorth);
  map->CreateObstacleAt(ObstacleType_Stool, 5, 1, kNorth);
  map->CreateObstacleAt(ObstacleType_Stool, 5, 2, kNorth);
  map->CreateObstacleAt(ObstacleType_Box02, 5, 5, kWest);
}

void Level1::TriggerAction() {}
