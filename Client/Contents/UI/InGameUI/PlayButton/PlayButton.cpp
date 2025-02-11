#include "PlayButton.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/GunfireButton/GunfireButton.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"

PlayButton::PlayButton(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();

  // PlayBtn
    _playBtnImgs[0] = CreateUI<UIImage>(L"PlayBtnImg_Act");
    _playBtnImgs[1] = CreateUI<UIImage>(L"PlayBtnImg_Hover");
    _playBtnImgs[2] = CreateUI<UIImage>(L"PlayBtnImg_Deact");

    _playBtnImgs[0]->SetSprite("2D\\UI\\UI_Play_Act.png", _pos);
    _playBtnImgs[1]->SetSprite("2D\\UI\\UI_Play_Hover.png", _pos);
    _playBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
    _playBtnImgs[2]->SetSprite("2D\\UI\\UI_Play_Deact.png", _pos);
    _playBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);

    _playBtn = CreateUI<UIButton>(L"PlayBtn");
    _playBtn->SetSize(_playBtnImgs[0]->GetSize());
    _playBtn->SetCenterPos(_pos);
#ifdef _DEBUG
    _playBtn->SetDebugDraw(true);
#endif // _DEBUG

    _playBtn->AddOnHoveredHandler([this]() {
      _playBtnImgs[0]->SetStatus(EStatus::EStatus_Inactive);
      _playBtnImgs[1]->SetStatus(EStatus::EStatus_Active);
      _playBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);
    });

    _playBtn->AddOnUnHoveredHandler([this]() {
      _playBtnImgs[0]->SetStatus(EStatus::EStatus_Active);
      _playBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
      _playBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);
    });

    _playBtn->AddOnClickHandler([this]() {

      _playBtn->Deactivate();

      if (_map)
      {
        _bPlayflag = true;

        // gunfireBtn 이 있고 gunfireBtn을 사용했을 때, ElizaAnim 출력
        _gunfireBtn = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                          ->GetUI<GunfireButton>(L"GunfireBtn");

        if (_gunfireBtn && _gunfireBtn->_bGunFireUseFlag == true)
        {
          _elizaAnim = _gunfireBtn->GetUI<UIAnim>(L"ElizaAnim");
          _elizaAnim->SetStatus(EStatus::EStatus_Active);
        }


        auto* inGameUi = _world->_canvas->GetPanel<InGameUI>(L"InGameUI");

        _map->TriggerAction();

        // AgentStorage 비활성화
        auto * agentStorage = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
            ->GetUI<AgentStorage>(L"AgentStorage");

        for (auto* agent : agentStorage->AgentList)
        {
          agent->_AgentBtn->Deactivate();
        }

        // GunfireButton 비활성화
        _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                               ->GetUI<GunfireButton>(L"GunfireBtn")
                               ->_gunfireBtn->Deactivate();
      }
    });
}

PlayButton::~PlayButton() {}

void PlayButton::Update(float dt)
{
  __super::Update(dt);

  if (_bPlayflag == true)
  {
    _playBtnImgs[0]->SetStatus(EStatus::EStatus_Inactive);
    _playBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
    _playBtnImgs[2]->SetStatus(EStatus::EStatus_Active);
  }
}

