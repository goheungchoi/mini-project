#include "GunfireButton.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"
#include "Shared/Config/Config.h"
#include "Contents/Levels/GameLevel.h"

GunfireButton::GunfireButton(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();
  _bHover = false;

  // Animation
  _ellizaAnim = CreateUI<UIAnim>(L"ElizaAnim");
  fs::path p = ns::kDataDir;
  std::string rootpath = p.string();
  std::string gunfire = "\\Eliza_Animation\\Eliza_Initiative_Gunfire.csv";
  _ellizaAnim->LoadAnimSprite("2D\\Animation\\Eliza_Initiative_Gunfire.png",
                              rootpath + gunfire);
  _ellizaAnim->SetCenterPos("Eliza_Initiative_Gunfire", {-200, 300});
  _ellizaAnim->SetScale("Eliza_Initiative_Gunfire", {.7f, .7f});
  _ellizaAnim->SetOpacity("Eliza_Initiative_Gunfire", 1.0f);
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
#endif

  _gunfireBtn->AddOnHoveredHandler([this]() { _bHover = true; });

  _gunfireBtn->AddOnUnHoveredHandler([this]() { _bHover = false; });

  _gunfireBtn->AddOnClickHandler([this]() {
    if (!_bGunFireUseFlag)
    {
      _bGunFireUseFlag = true;
      _map->TurnOnAssassinationMode();
    }
    //_bHover = false;
  });
}

GunfireButton::~GunfireButton() {}

void GunfireButton::Update(float dt)
{
  __super::Update(dt);



  _cursor = _world->_canvas->GetPanel<UICursor>(L"Cursor");

  // 커서 상태 업데이트
  if (_cursor)
  {
    _cursor->SetCursorType(_map->isAssassinationMode ? CursorType::SKILL
                                                     : CursorType::DEFAULT);
  }

  UpdateButtonState();

  // 플래그 업데이트 로직
  _bGunFireUseFlag =
      (_map->isAssassinationMode || _map->assassinationTarget) &&
      !(!_map->isAssassinationMode && !_map->assassinationTarget);
}

// 상태 관리 헬퍼 함수
void GunfireButton::UpdateButtonState()
{
  const bool activeCondition = _map->isAssassinationMode || _bGunFireUseFlag ||
                               _map->assassinationTarget;

  if (activeCondition)
  {
    _gunfireBtnImgs[0]->SetStatus(EStatus::EStatus_Inactive);
    _gunfireBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
    _gunfireBtnImgs[2]->SetStatus(EStatus::EStatus_Active);
  }
  else if (_bHover)
  {
    _gunfireBtnImgs[0]->SetStatus(EStatus::EStatus_Inactive);
    _gunfireBtnImgs[1]->SetStatus(EStatus::EStatus_Active);
    _gunfireBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);
  }
  else
  {
    _gunfireBtnImgs[0]->SetStatus(EStatus::EStatus_Active);
    _gunfireBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
    _gunfireBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);
  }
}
