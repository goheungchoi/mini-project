#include "GunfireButton.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"
#include "Shared/Config/Config.h"


GunfireButton::GunfireButton(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();

  // Animation
  _ellizaAnim = CreateUI<UIAnim>(L"ElizaAnim");
  fs::path p = ns::kDataDir;
  std::string rootpath = p.string();
  std::string gunfire = "\\Eliza_Animation\\Eliza_Initiative_Gunfire.csv";
  _ellizaAnim->LoadAnimSprite("2D\\Animation\\Eliza_Initiative_Gunfire.png",
                         rootpath + gunfire);
  _ellizaAnim->SetCenterPos("Eliza_Initiative_Gunfire", {0, 300});
  _ellizaAnim->SetScale("Eliza_Initiative_Gunfire", {.7f, .7f});
  _ellizaAnim->SetOpacity("Eliza_Initiative_Gunfire" , 1.0f);
  _ellizaAnim->SetCurrentAnimSprite("Eliza_Initiative_Gunfire");
  _ellizaAnim->SetStatus(EStatus_Inactive);


  // Image
  _gunfireBtnImgs[0] = CreateUI<UIImage>(L"OrderBtnImg_Act");
  _gunfireBtnImgs[1] = CreateUI<UIImage>(L"OrderBtnImg_Hover");
  _gunfireBtnImgs[2] = CreateUI<UIImage>(L"OrderBtnImg_Deact");

  _gunfireBtnImgs[0]->SetSprite("2D\\UI\\UI_Order_Act.png", {1770, 720});
  _gunfireBtnImgs[1]->SetSprite("2D\\UI\\UI_Order_Hover.png", {1770, 720});
  _gunfireBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
  _gunfireBtnImgs[2]->SetSprite("2D\\UI\\UI_Order_Deact.png", {1770, 720});
  _gunfireBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);

  // Button
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
  }
  else
  {
    _ellizaAnim->SetStatus(EStatus_Inactive);
    _gunfireBtnImgs[0]->SetStatus(EStatus::EStatus_Active);
    _gunfireBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
    _gunfireBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);
  }

  _cursor = _world->_canvas->GetPanel<UICursor>(L"Cursor");

  if (_cursor)
  {
    if (_map->isAssassinationMode)
    {
      _cursor->SetCursorType(CursorType::SKILL);
    }

    if (_bGunFireUseFlag && !(_map->isAssassinationMode))
    {
      _cursor->SetCursorType(CursorType::DEFAULT);
    }
  }

  if (!_map->isAssassinationMode && !_map->assassinationTarget)
  {
    _bGunFireUseFlag = false;
  }
  else if (!_map->isAssassinationMode && _map->assassinationTarget)
  {
    _bGunFireUseFlag = true;
  }

}
