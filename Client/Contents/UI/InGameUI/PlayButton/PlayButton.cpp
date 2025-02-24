#include "PlayButton.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"

#include "Contents/UI/DialogUI/ResultDialogUI.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "Contents/UI/InGameUI/GunfireButton/GunfireButton.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"

#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"

PlayButton::PlayButton(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();

  // PlayBtn
  _playBtnImgs[0] = CreateUI<UIImage>(L"PlayBtnImg_Act");
  _playBtnImgs[1] = CreateUI<UIImage>(L"PlayBtnImg_Hover");
  _playBtnImgs[2] = CreateUI<UIImage>(L"PlayBtnImg_Deact");

  _playBtnImgs[0]->SetSprite("2D\\UI\\UI_Play_Act.png", _pos);
  _playBtnImgs[1]->SetSprite("2D\\UI\\UI_Play_Hover.png", _pos);
  _playBtnImgs[2]->SetSprite("2D\\UI\\UI_Play_Deact.png", _pos);
  _playBtnImgs[0]->Activate();
  _playBtnImgs[1]->Activate();
  _playBtnImgs[2]->Activate();
  _playBtnImgs[0]->SetOpacity(1.f);
  _playBtnImgs[1]->SetOpacity(0.f);
  _playBtnImgs[2]->SetOpacity(0.f);

  _playBtn = CreateUI<UIButton>(L"PlayBtn");
  _playBtn->SetSize(_playBtnImgs[0]->GetSize());
  _playBtn->SetCenterPos(_pos);

#ifdef _DEBUG
  _playBtn->SetDebugDraw(true);
#endif // _DEBUG

  _playBtn->AddOnHoveredHandler([this]() {
    if (!_bHoverflag)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHoverflag = true;
    }
    _playBtnImgs[0]->SetOpacity(0.f);
    _playBtnImgs[1]->SetOpacity(1.f);
    _playBtnImgs[2]->SetOpacity(0.f);
  });

  _playBtn->AddOnUnHoveredHandler([this]() {
    _bHoverflag = false;
    _playBtnImgs[0]->SetOpacity(1.f);
    _playBtnImgs[1]->SetOpacity(0.f);
    _playBtnImgs[2]->SetOpacity(0.f);
  });

  _playBtn->AddOnClickHandler([this]() {
    SoundManager::PlaySound(SoundList::Button_Start);
    if (!_map->isPlacementModeOn)
    {
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
        auto* agentStorage = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                                 ->GetUI<AgentStorage>(L"AgentStorage");

        // GunfireButton 비활성화
        _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
            ->GetUI<GunfireButton>(L"GunfireBtn")
            ->_gunfireBtn->Deactivate();
      }
    }
  });

  SetOnActivatedEvent([=]() {
    _playBtn->Activate();
    _playBtnImgs[0]->Activate();
    _playBtnImgs[1]->Activate();
    _playBtnImgs[2]->Activate();
    _playBtnImgs[0]->SetOpacity(1.f);
    _playBtnImgs[1]->SetOpacity(0.f);
    _playBtnImgs[2]->SetOpacity(0.f);
    _playBtn->ClearHoverHandler();
    _playBtn->ClearUnHoverHandler();
    _playBtn->ClearClickHandler();
    _playBtn->AddOnHoveredHandler([this]() {
      if (!_bHoverflag)
      {
        SoundManager::PlaySound(SoundList::Button_Hover);
        _bHoverflag = true;
      }
      _playBtnImgs[0]->SetOpacity(0.f);
      _playBtnImgs[1]->SetOpacity(1.f);
      _playBtnImgs[2]->SetOpacity(0.f);
    });

    _playBtn->AddOnUnHoveredHandler([this]() {
      _bHoverflag = false;
      _playBtnImgs[0]->SetOpacity(1.f);
      _playBtnImgs[1]->SetOpacity(0.f);
      _playBtnImgs[2]->SetOpacity(0.f);
    });

    _playBtn->AddOnClickHandler([this]() {
      SoundManager::PlaySound(SoundList::Button_Start);
      if (!_map->isPlacementModeOn)
      {
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
          auto* agentStorage = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                                   ->GetUI<AgentStorage>(L"AgentStorage");

          // GunfireButton 비활성화
          _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
              ->GetUI<GunfireButton>(L"GunfireBtn")
              ->_gunfireBtn->Deactivate();
        }
      }
    });
  });
}

PlayButton::~PlayButton() {}

void PlayButton::BeginLevel()
{
  UIPanel::BeginLevel();
  SetOnActivatedEvent([=]() {

    _playBtnImgs[0]->SetOpacity(1.f);
    _playBtnImgs[1]->SetOpacity(0.f);
    _playBtnImgs[2]->SetOpacity(0.f);
    _playBtn->AddOnHoveredHandler([this]() {
      if (!_bHoverflag)
      {
        SoundManager::PlaySound(SoundList::Button_Hover);
        _bHoverflag = true;
      }
      _playBtnImgs[0]->SetOpacity(0.f);
      _playBtnImgs[1]->SetOpacity(1.f);
      _playBtnImgs[2]->SetOpacity(0.f);
    });

    _playBtn->AddOnUnHoveredHandler([this]() {
      _bHoverflag = false;
      _playBtnImgs[0]->SetOpacity(1.f);
      _playBtnImgs[1]->SetOpacity(0.f);
      _playBtnImgs[2]->SetOpacity(0.f);
    });

    _playBtn->AddOnClickHandler([this]() {
      SoundManager::PlaySound(SoundList::Button_Start);
      if (!_map->isPlacementModeOn)
      {
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
          auto* agentStorage = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                                   ->GetUI<AgentStorage>(L"AgentStorage");

          // GunfireButton 비활성화
          _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
              ->GetUI<GunfireButton>(L"GunfireBtn")
              ->_gunfireBtn->Deactivate();
        }
      }
    });

        _playBtn->Activate();
    _playBtnImgs[0]->Activate();
    _playBtnImgs[1]->Activate();
    _playBtnImgs[2]->Activate();
  });
}

void PlayButton::Update(float dt)
{
  __super::Update(dt);

  if (_bPlayflag == true || _map->isActionTriggered)
  {
    _playBtnImgs[0]->SetOpacity(0.f);
    _playBtnImgs[1]->SetOpacity(0.f);
    _playBtnImgs[2]->SetOpacity(1.f);
    _playBtn->Deactivate();
  }

  // if (_world->_canvas->GetPanel<ResultDialogUI>(L"ResultDialogUI")
  //         ->GetStatus() == EStatus::EStatus_Inactive)
  //{
  //   _bPlayflag = false;

  //  _playBtn->Activate();
  //  _playBtnImgs[0]->SetStatus(EStatus::EStatus_Active);
  //  _playBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
  //  _playBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);
  //}
}
