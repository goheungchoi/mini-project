#include "GunfireButton.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"
#include "Shared/Config/Config.h"
#include "Contents/Levels/GameLevel.h"
#include "Contents/UI/DialogUI/ResultDialogUI.h"

#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"

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
  _ellizaAnim->SetCenterPos("Eliza_Initiative_Gunfire", {-600, 300});
  _ellizaAnim->SetScale("Eliza_Initiative_Gunfire", {.7f, .7f});
  _ellizaAnim->SetOpacity("Eliza_Initiative_Gunfire", 1.0f);
  _ellizaAnim->SetCurrentAnimSprite("Eliza_Initiative_Gunfire");
  _ellizaAnim->SetStatus(EStatus_Inactive);
  _ellizaAnim->SetLateRender(true);
  // Image
  _gunfireBtnImgs[0] = CreateUI<UIImage>(L"OrderBtnImg_Act");
  _gunfireBtnImgs[1] = CreateUI<UIImage>(L"OrderBtnImg_Hover");
  _gunfireBtnImgs[2] = CreateUI<UIImage>(L"OrderBtnImg_Deact");

  _gunfireBtnImgs[0]->SetSprite("2D\\UI\\UI_Order_Act.png", _pos);
  _gunfireBtnImgs[1]->SetSprite("2D\\UI\\UI_Order_Hover.png", _pos);
  _gunfireBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
  _gunfireBtnImgs[2]->SetSprite("2D\\UI\\UI_Order_Deact.png", _pos);
  _gunfireBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);

  // Button
  _gunfireBtn = CreateUI<UIButton>(L"GunfireBtn");
  _gunfireBtn->SetSize(_gunfireBtnImgs[0]->GetSize());
  _gunfireBtn->SetCenterPos(_pos);
#ifdef _DEBUG
  _gunfireBtn->SetDebugDraw(true);
#endif

  _gunfireBtn->AddOnHoveredHandler([this]() {
    if (!_bHover)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHover = true;
    }
  });

  _gunfireBtn->AddOnUnHoveredHandler([this]() { _bHover = false; });

  _gunfireBtn->AddOnClickHandler([this]() {
    SoundManager::PlaySound(SoundList::Button_Click);

    if (!_bGunFireUseFlag)
    {
      _bGunFireUseFlag = true;
      _map->TurnOnAssassinationMode();
    }
    //_bHover = false;
  });
  SetOnActivatedEvent([=]() { _gunfireBtn->Activate(); });
}

GunfireButton::~GunfireButton() {}

void GunfireButton::BeginLevel()
{
  UIPanel::BeginLevel();

  SetOnActivatedEvent([=]() {
    _gunfireBtn->AddOnHoveredHandler([this]() {
      if (!_bHover)
      {
        SoundManager::PlaySound(SoundList::Button_Hover);
        _bHover = true;
      }
    });

    _gunfireBtn->AddOnUnHoveredHandler([this]() { _bHover = false; });

    _gunfireBtn->AddOnClickHandler([this]() {
      SoundManager::PlaySound(SoundList::Button_Click);

      if (!_bGunFireUseFlag)
      {
        _bGunFireUseFlag = true;
        _map->TurnOnAssassinationMode();
      }
      //_bHover = false;
    });
    _gunfireBtn->Activate();
    _gunfireBtn->SetDebugDraw(true);
  });

}

void GunfireButton::Update(float dt)
{
  __super::Update(dt);

  _cursor = _world->_canvas->GetPanel<UICursor>(L"Cursor");

  // 커서 상태 업데이트
  if (_cursor)
  {
    if (_map->isAssassinationMode)
    {
      _cursor->SetCursorType(CursorType::SKILL);
    }
    else
    {
      _cursor->SetCursorType(CursorType::DEFAULT);
    }
  }

  UpdateButtonState();

  // 플래그 업데이트 로직
  _bGunFireUseFlag =
      (_map->isAssassinationMode || _map->assassinationTarget) &&
      !(!_map->isAssassinationMode && !_map->assassinationTarget);


  // ResultDialogUI 가 꺼지면 GunfireBtn 활성화
  //if (_world->_canvas->GetPanel<ResultDialogUI>(L"ResultDialogUI")
  //        ->GetStatus() == EStatus::EStatus_Inactive)
  //{
  //  _bGunFireUseFlag = false;

  //  _gunfireBtn->Activate();
  //}
}

// 상태 관리 헬퍼 함수
void GunfireButton::UpdateButtonState()
{
  static bool activeCondition{false};

  if (!_map->isActionTriggered)
  {
    activeCondition = _map->isAssassinationMode || _bGunFireUseFlag ||
                      _map->assassinationTarget;
  }

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
