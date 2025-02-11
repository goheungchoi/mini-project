#include "MainMenu.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Resource2DManager/Resource2DManager.h"

#include "Contents/UI/MainMenuUI/MainMenuUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"

#include "Contents/UI/TransitionUI/TransitionUI.h"
#include "GameFramework/UI/UIImage/UIImage.h"

void MainMenu::PrepareLevel()
{
  // UI Resource Load
#ifdef USED2D
  //Resource2DManager::GetInstance()->LoadSprite("Textures\\TitleLogo.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Title\\Title_Background.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Title\\Title_Background2.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Title\\Title_Button_01_Basic.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Title\\Title_Button_01_Hover.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Title\\Title_Button_02_Basic.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Title\\Title_Button_02_Hover.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Title\\Title_Button_03_Basic.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Title\\Title_Button_03_Hover.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\FadeBlack.png");
#endif // USED2D
}

void MainMenu::BeginLevel()
{
#ifdef USED2D
  mainmenuUI = world->_canvas->CreatePanel<MainMenuUI>(L"MainMenuUI");

  transitionUI = world->_canvas->CreatePanel<TransitionUI>(L"FadeTransition");
  transitionUI->_blackImage->SetOpacity(1.0f);
  transitionUI->FadeIn(2.f);
#endif // USED2D
}

void MainMenu::CleanupLevel() {
}

void MainMenu::SetEndingFlag(int i)
{
  mainmenuUI->SetEndingFlag(i);
}

void MainMenu::CreateMap() {}

void MainMenu::TriggerAction() {}
