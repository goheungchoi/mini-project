#include "RetryButton.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/Levels/GameLevel.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/GunfireButton/GunfireButton.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/PlayButton/PlayButton.h"

#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"

RetryButton::RetryButton(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();
  _retryBtnImgs[0] = CreateUI<UIImage>(L"RetryBtnImg_Act");
  _retryBtnImgs[1] = CreateUI<UIImage>(L"RetryBtnImg_Hover");

  _retryBtnImgs[0]->SetSprite("2D\\UI\\UI_Retry_Act.png", {1580, 140});
  _retryBtnImgs[1]->SetSprite("2D\\UI\\UI_Retry_Hover.png", {1580, 140});
  _retryBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);

  // Button
  _retryBtn = CreateUI<UIButton>(L"RetryBtn");
  _retryBtn->SetSize(_retryBtnImgs[0]->GetSize());
  _retryBtn->SetCenterPos({1580, 140});
#ifdef _DEBUG
  _retryBtn->SetDebugDraw(true);
#endif // _DEBUG

  _retryBtn->AddOnClickHandler([this]() {
    if (_map->isPlacementModeOn)
    SoundManager::PlaySound(SoundList::Button_Click);
    int levelIdx =
        static_cast<GameLevel*>(_world->GetCurrentLevel())->GetStageIdx();
    bool IsGunfireAvailableStage =
        (levelIdx == 5 || levelIdx == 6 || levelIdx == 9 || levelIdx == 12 ||
         levelIdx == 13 || levelIdx == 14 || levelIdx == 15 || levelIdx == 16);

	  _map->ResetGame();

    if (IsGunfireAvailableStage)
    {

      // GunfireButton 활성화
      auto* gunfireBtn = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                             ->GetUI<GunfireButton>(L"GunfireBtn");

      if (gunfireBtn)
      {
        if (gunfireBtn->_bGunFireUseFlag == true)
        {
          gunfireBtn->_bGunFireUseFlag = false;
        }

        for (const auto& [childName, child] : gunfireBtn->uiMap)
        {
          child->Activate();
        }

        gunfireBtn->GetUI<UIAnim>(L"ElizaAnim")->Deactivate();
      }

      _world->_canvas->GetPanel<InGameUI>(L"InGameUI")->ShowUI(L"GunfireBtn");
    }


    // AgentStorage 활성화
    auto* agentStorage = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                             ->GetUI<AgentStorage>(L"AgentStorage");
    _world->_canvas->GetPanel<InGameUI>(L"InGameUI")->ShowUI(L"AgentStorage");

    // PlayBtn 활성화
    auto* playBtn = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                        ->GetUI<PlayButton>(L"PlayBtn");
    if (playBtn)
    {
      playBtn->GetUI<UIImage>(L"PlayBtnImg_Act")->Activate();
      playBtn->GetUI<UIImage>(L"PlayBtnImg_Hover")->Deactivate();
      playBtn->GetUI<UIImage>(L"PlayBtnImg_Deact")->Deactivate();
      playBtn->GetUI<UIButton>(L"PlayBtn")->Activate();

      playBtn->Activate();
      playBtn->_bPlayflag = false;
    }

    _bHoverFlag = false;

  });

  _retryBtn->AddOnHoveredHandler([this]() { 
    if (!_bHoverFlag)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHoverFlag = true;
    }
  });

  _retryBtn->AddOnUnHoveredHandler([this]() { _bHoverFlag = false; });


  SetOnActivatedEvent([=]() {
    _retryBtnImgs[0]->SetStatus(EStatus::EStatus_Active);
    _retryBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
    _retryBtn->Activate();
  });


}

RetryButton::~RetryButton() {}

void RetryButton::Update(float dt)
{
  __super::Update(dt);

  if (!_bHoverFlag)
  {
    _retryBtnImgs[0]->SetStatus(EStatus::EStatus_Active);
    _retryBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
  }
  else
  {
    _retryBtnImgs[0]->SetStatus(EStatus::EStatus_Inactive);
    _retryBtnImgs[1]->SetStatus(EStatus::EStatus_Active);
  }
}
