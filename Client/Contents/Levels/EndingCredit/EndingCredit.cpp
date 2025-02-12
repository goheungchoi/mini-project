#include "EndingCredit.h"
#include "Resource2DManager/Resource2DManager.h"
#include "Contents/UI/TransitionUI/TransitionUI.h"
#include "Contents/UI/CreditUI/CreditUI.h"
#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIImage/UIImage.h"

void EndingCredit::PrepareLevel()
{
  // UI Resource Load
#ifdef USED2D
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Credit_Final.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\Credit_Back.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\UI_vignette_01.png");
  Resource2DManager::GetInstance()->LoadSprite("2D\\UI\\FadeBlack.png");
#endif // USED2D
}

void EndingCredit::BeginLevel()
{
  SoundManager::StopSound(SoundList::Background_Pub_Ambient);
  SoundManager::StopSound(SoundList::Background_Pub);
  SoundManager::StopSound(SoundList::Background_ConferenceHall_Ambient);
  SoundManager::StopSound(SoundList::Background_ConferenceHall);
  SoundManager::StopSound(SoundList::Background_Storage_Ambient);
  SoundManager::StopSound(SoundList::Background_Storage);
  SoundManager::StopSound(SoundList::Background_Dialog);

  _creditUI = world->_canvas->CreatePanel<CreditUI>(L"CreditUI");

  _transitionUI = world->_canvas->CreatePanel<TransitionUI>(L"FadeTransition");
  _transitionUI->_blackImage->SetOpacity(1.0f);
  _transitionUI->FadeIn(2.f);

  _transitionUI->OnFadeOutDone = [this]() { world->CommitLevelChange(); };
}

void EndingCredit::DestroyLevel()
{
  _transitionUI->FadeOut(1.0f);
	Level::DestroyLevel();
}

void EndingCredit::CleanupLevel() {}

void EndingCredit::CreateMap() {}

void EndingCredit::TriggerAction() {}
