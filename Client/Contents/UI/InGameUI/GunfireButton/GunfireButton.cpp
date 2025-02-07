#include "GunfireButton.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIImage/UIImage.h"

GunfireButton::GunfireButton(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();

  _gunfireBtnImgs[0] = CreateUI<UIImage>(L"OrderBtnImg_Act");
  _gunfireBtnImgs[1] = CreateUI<UIImage>(L"OrderBtnImg_Hover");
  _gunfireBtnImgs[2] = CreateUI<UIImage>(L"OrderBtnImg_Deact");

  _gunfireBtnImgs[0]->SetSprite("2D\\UI\\UI_Order_Act.png", {1770, 720});
  _gunfireBtnImgs[1]->SetSprite("2D\\UI\\UI_Order_Hover.png", {1770, 720});
  _gunfireBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
  _gunfireBtnImgs[2]->SetSprite("2D\\UI\\UI_Order_Deact.png", {1770, 720});
  _gunfireBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);

  _gunfireBtn = CreateUI<UIButton>(L"GunfireBtn");
  _gunfireBtn->SetSize(_gunfireBtnImgs[0]->GetSize());
  _gunfireBtn->SetCenterPos({1770, 720});
#ifdef _DEBUG
  _gunfireBtn->SetDebugDraw(true);
#endif // _DEBUG

  _gunfireBtn->AddOnHoveredHandler([this]() {
    _gunfireBtnImgs[0]->SetStatus(EStatus::EStatus_Inactive);
    _gunfireBtnImgs[1]->SetStatus(EStatus::EStatus_Active);
    _gunfireBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);
  });

  _gunfireBtn->AddOnUnHoveredHandler([this]() {
    _gunfireBtnImgs[0]->SetStatus(EStatus::EStatus_Active);
    _gunfireBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
    _gunfireBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);
  });

  _gunfireBtn->AddOnClickHandler([this]() {
    if (_bGunFireUseFlag == false)
    {
      _bGunFireUseFlag = true;
      _map->TurnOnAssassinationMode();
    }
  });
}

GunfireButton::~GunfireButton() {}

void GunfireButton::Update(float dt)
{
  __super::Update(dt);

  if (_bGunFireUseFlag)
  {
    _gunfireBtnImgs[0]->SetStatus(EStatus::EStatus_Inactive);
    _gunfireBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
    _gunfireBtnImgs[2]->SetStatus(EStatus::EStatus_Active);

    _cursor = _world->_canvas->GetPanel<UICursor>(L"Cursor");

    if (_cursor)
    {
      _cursor->SetCursorType(CursorType::SKILL);
    }

    //if (_map->isAssassinationMode)
    //{
    //  _map->assassinationTarget = _map->hoveredCharacter;
    //  _map->isAssassinationMode = false;
    //}
  }
}
