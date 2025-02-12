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
      "2D\\UI\\Title\\Title_Background2.png");
#endif // USED2D
}

void ChallengeMode::BeginLevel()
{
#ifdef USED2D
  challengeModeUI =
      world->_canvas->CreatePanel<ChallengeModeUI>(L"ChallengeModeUI");
#endif
}

void ChallengeMode::CleanupLevel() {}


void ChallengeMode::CreateMap() {}

void ChallengeMode::TriggerAction() {}
