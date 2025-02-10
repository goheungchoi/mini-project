#include "RetryButton.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/GunfireButton/GunfireButton.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/PlayButton/PlayButton.h"

RetryButton::RetryButton(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();
  _retryBtnImgs[0] = CreateUI<UIImage>(L"RetryBtnImg_Act");
  _retryBtnImgs[0]->SetSprite("2D\\UI\\UI_Retry_Act.png", {1600, 100});

  // Button
  _retryBtn = CreateUI<UIButton>(L"RetryBtn");
  _retryBtn->SetSize(_retryBtnImgs[0]->GetSize());
  _retryBtn->SetCenterPos({1600, 100});
#ifdef _DEBUG
  _retryBtn->SetDebugDraw(true);
#endif // _DEBUG

  _retryBtn->AddOnClickHandler([this]() {
	  _map->ResetGame();
      
    // GunfireButtonr과 AgentStorage 활성화 하기.
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


    auto* agentStorage = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                             ->GetUI<AgentStorage>(L"AgentStorage");
    for (auto* agent : agentStorage->AgentList)
    {
      agent->_AgentBtn->Activate();
    }

    _world->_canvas->GetPanel<InGameUI>(L"InGameUI")->ShowUI(L"GunfireBtn");
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

  });

  _retryBtn->AddOnHoveredHandler([this]() {
    _retryBtnImgs[0]->SetOpacity(0.8f);
  });

  _retryBtn->AddOnUnHoveredHandler([this]() {
    _retryBtnImgs[0]->SetOpacity(1.0f);
  });

}

RetryButton::~RetryButton() {}

void RetryButton::Update(float dt)
{
  __super::Update(dt);
}
