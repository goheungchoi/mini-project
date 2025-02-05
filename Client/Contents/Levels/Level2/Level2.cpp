#include "Level2.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Resource2DManager/Resource2DManager.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"

void Level2::PrepareLevel()
{
  // UI Resource Load
  Resource2DManager::GetInstance()->LoadSprite(
      "Textures\\MainMissonUI_Test.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "Textures\\SubMissonUI_Test.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\SnipingUI_Test.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\Btn_Slot.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\Btn_Slot2.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\Btn_Play.png");
}

void Level2::BeginLevel()
{
  world->_canvas->CreatePanel<InGameUI>(L"InGameUI");
}

void Level2::CleanupLevel() {}

void Level2::CreateMap() {}

void Level2::TriggerAction() {}
