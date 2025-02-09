#include "PlayButton.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"

#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/GunfireButton/GunfireButton.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"

PlayButton::PlayButton(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();

  // PlayBtn
    _playBtnImgs[0] = CreateUI<UIImage>(L"PlayBtnImg_Act");
    _playBtnImgs[1] = CreateUI<UIImage>(L"PlayBtnImg_Hover");
    _playBtnImgs[2] = CreateUI<UIImage>(L"PlayBtnImg_Deact");

    _playBtnImgs[0]->SetSprite("2D\\UI\\UI_Play_Act.png", {1800, 100});
    _playBtnImgs[1]->SetSprite("2D\\UI\\UI_Play_Hover.png", {1800, 100});
    _playBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
    _playBtnImgs[2]->SetSprite("2D\\UI\\UI_Play_Deact.png", {1800, 100});
    _playBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);

    _playBtn = CreateUI<UIButton>(L"PlayBtn");
    _playBtn->SetSize(_playBtnImgs[0]->GetSize());
    _playBtn->SetCenterPos({1800, 100});
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
      if (_map)
      {
        _map->TriggerAction();
        _bPlayflag = true;

        auto* gunfireBtn = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                       ->GetUI<GunfireButton>(L"GunfireBtn");

        if (gunfireBtn && gunfireBtn->_bGunFireUseFlag == true)
        {
          gunfireBtn->GetUI<UIAnim>(L"ElizaAnim")
              ->SetStatus(EStatus::EStatus_Active);
        }
      }
    });
}

PlayButton::~PlayButton() {}

void PlayButton::Update(float dt)
{
  __super::Update(dt);

  if (GetStatus() == EStatus::EStatus_Active)
  {
    _bPlayflag = false;
  }

  if (_bPlayflag == true)
  {
    _playBtnImgs[0]->SetStatus(EStatus::EStatus_Inactive);
    _playBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
    _playBtnImgs[2]->SetStatus(EStatus::EStatus_Active);
  }


}
