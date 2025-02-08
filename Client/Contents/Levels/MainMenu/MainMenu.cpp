#include "MainMenu.h"
#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Resource2DManager/Resource2DManager.h"

#include "Contents/UI/MainMenuUI/MainMenuUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"

#include "Contents/UI/TransitionUI/TransitionUI.h"
#include "GameFramework/UI/UIImage/UIImage.h"

ModelHandle enemyBrawlerModelHandle;
ModelHandle enemyGunmanModelHandle;

ModelHandle allyBrawlerModelHandle;
ModelHandle allySlasherModelHandle;
ModelHandle allyGunmanModelHandle;

void MainMenu::PrepareLevel()
{
  // Character model loadings.
  enemyBrawlerModelHandle =
      LoadModel("Models\\Character\\Enemy\\Enemy_Punch\\Enemy_Punch.glb");
  enemyGunmanModelHandle =
      LoadModel("Models\\Character\\Enemy\\EnemyGunman\\EnemyGunman.glb");

  allyBrawlerModelHandle =
      LoadModel("Models\\Character\\Player\\Player_Punch\\Player_Punch.glb");
  allySlasherModelHandle =
      LoadModel("Models\\Character\\Player\\Player_Knife\\Player_Knife.glb");
  allyGunmanModelHandle =
      LoadModel("Models\\Character\\Player\\Player_Gun\\Player_Gun.glb");

  // UI Resource Load
#ifdef USED2D
  Resource2DManager::GetInstance()->LoadSprite("Textures\\TitleBackGround.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\TitleLogo.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\StartBtn.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\StartBtn_hover.png");
  Resource2DManager::GetInstance()->LoadSprite("Textures\\StartBtn_click.png");

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\FadeBlack.png");
#endif // USED2D
}

void MainMenu::BeginLevel()
{

  GetWorld()->PrepareChangeLevel("Level1");
#ifdef USED2D
  world->_canvas->CreatePanel<MainMenuUI>(L"MainMenuUI");

  transitionUI = world->_canvas->CreatePanel<TransitionUI>(L"FadeTransition");
  transitionUI->_blackImage->SetOpacity(1.0f);
  transitionUI->FadeIn(2.f);
#endif // USED2D
}

void MainMenu::CleanupLevel() {
  UnloadModel(enemyBrawlerModelHandle);
  UnloadModel(enemyGunmanModelHandle);
  UnloadModel(allyBrawlerModelHandle);
  UnloadModel(allySlasherModelHandle);
  UnloadModel(allyGunmanModelHandle);
}

void MainMenu::CreateMap() {}

void MainMenu::TriggerAction() {}
