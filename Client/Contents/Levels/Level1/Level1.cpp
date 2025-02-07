#include "Level1.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Resource2DManager/Resource2DManager.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/GameObjects/Map/Characters/Character.h"


void Level1::PrepareLevel()
{
  __super::PrepareLevel();
}

void Level1::BeginLevel()
{
  __super::BeginLevel();

  #ifdef USED2D
  inGameUI->HideUI(L"GunfireBtn");
  inGameUI->_agentStorage->SetAgent(kBrawler, {1400, 960});
  inGameUI->_agentStorage->SetAgent(kSlasher, {1600, 960});
  inGameUI->_agentStorage->SetAgent(kGunman, {1800, 960});
  #endif
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


}

void Level1::TriggerAction() {}
