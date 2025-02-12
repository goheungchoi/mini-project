#include "DialogLevel.h"

#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/UI/DialogUI/DialogUI.h"
#include "Contents/UI/TransitionUI/TransitionUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "Resource2DManager/Resource2DManager.h"

void DialogLevel::PrepareLevel()
{
  Level::PrepareLevel();
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

  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\FadeBlack.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Textbox_01.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Textbox_Button.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_Selectbox_01.png");
}

void DialogLevel::BeginLevel()
{
  Level::BeginLevel();
  //gameManager = world->CreateGameObject<GameManager>();
#ifdef USED2D
  dialogUI =world->_canvas->CreatePanel<DialogUI>(L"ElizaDialog");
  dialogUI->StageIdx = _stageidx;
  dialogUI->_prevBattleResult = _battleResult;
  dialogUI->SetStageDialogIndex();
  world->_canvas->CreatePanel<UICursor>(L"Cursor");
  transitionUI = world->_canvas->CreatePanel<TransitionUI>(L"FadeTransition");
  transitionUI->_blackImage->SetOpacity(1.0f);
  transitionUI->FadeIn(2.f);


#endif // USED2D
}

void DialogLevel::CleanupLevel()
{
  Level::CleanupLevel();
}

void DialogLevel::DestroyLevel()
{

  transitionUI->FadeOut(2.f);
  Level::DestroyLevel();
}

void DialogLevel::SetStageIdx(int stageIdx)
{
  _stageidx = stageIdx;
}

void DialogLevel::SetBattleResult(eBattleResult battleresult)
{
  _battleResult = battleresult;
}