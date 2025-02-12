#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "ChallengeMode.h"
#include "Resource2DManager/Resource2DManager.h"

#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIPanel/UIPanel.h"
#include "Contents/UI/TransitionUI/TransitionUI.h"
#include "GameFramework/UI/UIImage/UIImage.h"

void ChallengeMode::PrepareLevel()
{
#ifdef USED2D
  // Resource2DManager::GetInstance()->LoadSprite("Textures\\TitleLogo.png");
  Resource2DManager::GetInstance()->LoadSprite(
      "2D\\UI\\Title\\Challenge_BackGround.png");
#endif // USED2D
}

void ChallengeMode::BeginLevel()
{
#ifdef USED2D
  transitionUI = world->_canvas->CreatePanel<TransitionUI>(L"FadeTransition");
  transitionUI->_blackImage->SetOpacity(1.0f);
  transitionUI->FadeIn(2.f);
#endif
}

void ChallengeMode::CleanupLevel() {}


void ChallengeMode::CreateMap() {}

void ChallengeMode::TriggerAction() {}
