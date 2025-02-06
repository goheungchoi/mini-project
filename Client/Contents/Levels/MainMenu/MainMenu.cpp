#include "MainMenu.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Resource2DManager/Resource2DManager.h"

#include "Contents/UI/MainMenuUI/MainMenuUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"

void MainMenu::PrepareLevel()
{
  // UI Resource Load
#ifdef USED2D
  Resource2DManager::GetInstance()->LoadSprite("Textures\\TitleBackGround.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\TitleLogo.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\StartBtn.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\StartBtn_hover.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\StartBtn_click.png");
#endif // USED2D
}

void MainMenu::BeginLevel()
{
#ifdef USED2D
  world->_canvas->CreatePanel<MainMenuUI>(L"MainMenuUI");
#endif // USED2D
}

void MainMenu::CleanupLevel() {}

void MainMenu::CreateMap() {}

void MainMenu::TriggerAction() {}
