#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "ChallengeMode.h"
#include "Resource2DManager/Resource2DManager.h"

#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"
#include "Contents/UI/TransitionUI/TransitionUI.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "Contents/UI/ChallengeModeUI/ChallengeModeUI.h"
void ChallengeMode::PrepareLevel()
{
#ifdef USED2D
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\StageNum\\ChallengeBG.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\FadeBlack.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\StageNum\\StageNum_Hover_01.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\StageNum\\StageNum_Act_01.png");
#endif // USED2D
}

void ChallengeMode::BeginLevel()
{
#ifdef USED2D
  challengeModeUI =
      world->_canvas->CreatePanel<ChallengeModeUI>(L"ChallengeModeUI");
  transitionUI = world->_canvas->CreatePanel<TransitionUI>(L"FadeTransition");
  transitionUI->_blackImage->SetOpacity(1.0f);
  transitionUI->FadeIn(2.f);
  transitionUI->OnFadeOutDone = [this]() { world->CommitLevelChange(); };
#endif
}

void ChallengeMode::CleanupLevel() {}


void ChallengeMode::CreateMap() {}

void ChallengeMode::TriggerAction() {}
