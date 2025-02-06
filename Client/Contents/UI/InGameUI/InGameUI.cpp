#include "InGameUI.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"

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

  // SnipingBtn
  _snipingBtnImg = CreateUI<UIImage>(L"SnipingBtnImg");
  _snipingBtnImg->SetSprite("2D\\UI\\UI_Order_Act.png", {1770, 720});

  // CombatBtn
  _combatBtnImg = CreateUI<UIImage>(L"CombatBtnImg");
  _combatBtnImg->SetSprite("2D\\UI\\UI_Storage_Act_Fist.png", {1600, 960});

  // PlayBtn
  {
    _playBtnImgList[0] = CreateUI<UIImage>(L"PlayBtnImg_Act");
    _playBtnImgList[1] = CreateUI<UIImage>(L"PlayBtnImg_Hover");
    _playBtnImgList[2] = CreateUI<UIImage>(L"PlayBtnImg_Deact");

    _playBtnImgList[0]->SetSprite("2D\\UI\\UI_Play_Act.png", {1800, 100});
    _playBtnImgList[1]->SetSprite("2D\\UI\\UI_Play_Hover.png", {1800, 100});
    _playBtnImgList[1]->SetStatus(EStatus::EStatus_Inactive);
    _playBtnImgList[2]->SetSprite("2D\\UI\\UI_Play_Deact.png", {1800, 100});
    _playBtnImgList[2]->SetStatus(EStatus::EStatus_Inactive);

    _playBtn = CreateUI<UIButton>(L"PlayBtn");
    _playBtn->SetSize(_playBtnImgList[0]->GetSize());
    _playBtn->SetCenterPos({1800, 100});
#ifdef _DEBUG
    _playBtn->SetDebugDraw(true);
#endif // _DEBUG

    _playBtn->AddOnHoveredHandler([this]() {
      _playBtnImgList[0]->SetStatus(EStatus::EStatus_Inactive);
      _playBtnImgList[1]->SetStatus(EStatus::EStatus_Active);
      _playBtnImgList[2]->SetStatus(EStatus::EStatus_Inactive);
    });

    _playBtn->AddOnUnHoveredHandler([this]() {
      _playBtnImgList[0]->SetStatus(EStatus::EStatus_Active);
      _playBtnImgList[1]->SetStatus(EStatus::EStatus_Inactive);
      _playBtnImgList[2]->SetStatus(EStatus::EStatus_Inactive);
    });

    _playBtn->AddOnClickHandler([this]() {
      auto* map = _world->FindGameObjectByType<Map>();
      if (map)
      {
        map->TriggerAction();
      }
    });
  }
}

InGameUI::~InGameUI() {}
