#include "InGameUI.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"
#include "GameFramework/UI/UICursor/UICursor.h"

InGameUI::InGameUI(World* world) : UIPanel(world)
{

  // MainMisson
  _mainMissonImg = CreateUI<UIImage>(L"MainMissonImg");
  _mainMissonImg->SetSprite("2D\\UI\\UI_Stage_L.png", {338, 100});

  _mainMissonTxt = CreateUI<UIText>(L"MainMissonTxt");
  _mainMissonTxt->SetCenterPos({300, 130});
  _mainMissonTxt->SetSize(
      {_mainMissonImg->GetSize().x, _mainMissonImg->GetSize().y});
  _mainMissonTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  //_mainMissonTxt->SetFont(L"강원교육모두 Bold");
  _mainMissonTxt->SetFont(L"강원교육모두 굵게");
  _mainMissonTxt->SetText(
      L"사라진 아이들에 대한 단서 찾기\n모든 적 처치 (0/8)");
  _mainMissonTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _mainMissonTxt->SetDebugDraw(false);
#endif // _DEBUG

  // SubMisson
  _subMissonImg = CreateUI<UIImage>(L"MainMissonImg");
  _subMissonImg->SetSprite("2D\\UI\\UI_Stage_S.png", {208, 240});

  // CombatBtn
  _combatBtnImg = CreateUI<UIImage>(L"CombatBtnImg");
  _combatBtnImg->SetSprite("2D\\UI\\UI_Storage_Act_Fist.png", {1600, 960});

  // OrderBtn
  {
    _orderBtnImgs[0] = CreateUI<UIImage>(L"OrderBtnImg_Act");
    _orderBtnImgs[1] = CreateUI<UIImage>(L"OrderBtnImg_Hover");
    _orderBtnImgs[2] = CreateUI<UIImage>(L"OrderBtnImg_Deact");

    _orderBtnImgs[0]->SetSprite("2D\\UI\\UI_Order_Act.png", {1770, 720});
    _orderBtnImgs[1]->SetSprite("2D\\UI\\UI_Order_Hover.png", {1770, 720});
    _orderBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
    _orderBtnImgs[2]->SetSprite("2D\\UI\\UI_Order_Deact.png", {1770, 720});
    _orderBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);

    _orderBtn = CreateUI<UIButton>(L"OrderBtn");
    _orderBtn->SetSize(_orderBtnImgs[0]->GetSize());
    _orderBtn->SetCenterPos({1770, 720});
#ifdef _DEBUG
    _orderBtn->SetDebugDraw(true);
#endif // _DEBUG

    _orderBtn->AddOnHoveredHandler([this]() {
      _orderBtnImgs[0]->SetStatus(EStatus::EStatus_Inactive);
      _orderBtnImgs[1]->SetStatus(EStatus::EStatus_Active);
      _orderBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);
    });

    _orderBtn->AddOnUnHoveredHandler([this]() {
      _orderBtnImgs[0]->SetStatus(EStatus::EStatus_Active);
      _orderBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
      _orderBtnImgs[2]->SetStatus(EStatus::EStatus_Inactive);
    });
  }

  // PlayBtn
  {
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
      auto* map = _world->FindGameObjectByType<Map>();
      if (map)
      {
        map->TriggerAction();
      }
    });
  }

    _cursor = CreateUI<UICursor>(L"Cursor");
  _cursor->SetCursorType(CursorType::SKILL);

}

InGameUI::~InGameUI() {}
