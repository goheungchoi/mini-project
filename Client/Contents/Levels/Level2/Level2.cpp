#include "Level2.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/UI/DialogUI/DialogUI.h"
#include "Resource2DManager/Resource2DManager.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"
#include "GameFramework/UI/UICursor/UICursor.h"

void Level2::PrepareLevel()
{
  // UI Resource Load


  Resource2DManager::GetInstance()->LoadSprite(
      "Textures\\Picture\\PhotoPanel.png");

  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\Animation\\Eliza_Initiative_Gunfire.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\Animation\\Eliza_Noble.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\Animation\\Eliza_Resistance_Despair30.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\Animation\\Eliza_Resistance_Despair70.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\Animation\\Eliza_Resistance_Joy.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\Animation\\Eliza_Resistance_Sad.png");
}

void Level2::BeginLevel()
{
#ifdef USED2D
  world->_canvas->CreatePanel<DialogUI>(L"ElizaDialog");
world->_canvas->CreatePanel<UICursor>(L"Cursor");
#endif // USED2D
}

void Level2::CleanupLevel() {}

void Level2::CreateMap() {}

void Level2::TriggerAction() {}
