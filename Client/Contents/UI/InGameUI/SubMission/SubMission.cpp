#include "SubMission.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"

#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "Contents/Levels/GameLevel.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/InGameUI/RetryButton/RetryButton.h"
#include "Contents/UI/InGameUI/ApplyButton/ApplyButton.h"

SubMission::SubMission(World* world, Vector2 pos) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();
  _pos = pos;

  // SubMisson
  _subMissionImg = CreateUI<UIImage>(L"SubMissonImg");
  _subMissionImg->SetSprite("2D\\UI\\UI_Stage_S.png", _pos);

  _failImg = CreateUI<UIImage>(L"FailImg");
  _failImg->SetSprite("Textures\\X_test.png", _pos);
  _failImg->SetLateRender(true);
  _failImg->Deactivate();

  StageIdx = static_cast<GameLevel*>(_world->GetCurrentLevel())->GetStageIdx();

}

SubMission::~SubMission() {}

void SubMission::Update(float dt)
{

//if (StageIdx == 5)
//{
//  if (_map)
//  {
//    // 민간인과 대원의 사망자 수의 합이 1 이상이라면
//    int deadAlliesNum = _map->GetNumDeadAllies();
//    int deadCiviliansNum = _map->GetNumDeadCivilians();
//    int totalDeadNum = deadAlliesNum + deadCiviliansNum;
//
//    if (totalDeadNum >= 1)
//    {
//      // 미션 실패 이미지 컴포넌트 활성화 한다.
//      _failImg->Activate();
//    }
//  }
//}

if (StageIdx == 7)
{
  if (_map)
  {
    int deadCiviliansNum = _map->GetNumDeadCivilians();
    int deadAlliesNum = _map->GetNumDeadAllies();

    if (deadCiviliansNum >= 1)  // 부관(엘리자)을 희생시키면
    {
      // 미션 실패 이미지 컴포넌트 활성화 한다.
      auto* sub_ElizaDead = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                                 ->GetUI<SubMission>(L"SubMission");

      if (sub_ElizaDead)
      {
        sub_ElizaDead->_failImg->Activate();
      }
    }
    else if (deadAlliesNum >= 2)  // 대원 2명을 희생시키면
    {
      // 미션 실패 이미지 컴포넌트 활성화 한다.
      auto* sub_AlliesDead = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
          ->GetUI<SubMission>(L"SubMission_2");

      if (sub_AlliesDead)
      {
        sub_AlliesDead->_failImg->Activate();
      }
    }
  }
}
  

  // Retry 버튼이 눌렸거나 Apply 버튼이 눌렸다면  _failImg->Deactivate() 해라.
  auto* RetryBtn = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                 ->GetUI<RetryButton>(L"RetryBtn");

  auto* ApplyBtn = _world->_canvas->GetPanel<InGameUI>(L"InGameUI")
                       ->GetUI<ApplyButton>(L"ApplyBtn");

  if (RetryBtn || ApplyBtn)
  {
    if (RetryBtn->GetStatus() == EStatus_Inactive ||
        ApplyBtn->GetStatus() == EStatus_Inactive)
    {
      _failImg->Deactivate();
    }
  }

}

void SubMission::SetText(std::wstring text)
{
  Vector2 textPos = {_pos.x + 140, _pos.y + 57};

  _subMissionTxt = CreateUI<UIText>(L"SubMissionTxt");
  _subMissionTxt->SetCenterPos({textPos});
  _subMissionTxt->SetSize(
      {_subMissionImg->GetSize().x, _subMissionImg->GetSize().y});
  _subMissionTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _subMissionTxt->SetFont(L"PT Noeul");
  _subMissionTxt->SetFontSize(21.0f);
  _subMissionTxt->SetFontWeight(FontWeight::NORMAL);
  _subMissionTxt->SetText(text.c_str());
  _subMissionTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _subMissionTxt->SetDebugDraw(false);
#endif // _DEBUG

  _selectTxt = CreateUI<UIText>(L"SubSelectTxt");
  _selectTxt->SetCenterPos({textPos.x - 60, textPos.y});
  _selectTxt->SetSize(
      {_subMissionImg->GetSize().x, _subMissionImg->GetSize().y});
  _selectTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _selectTxt->SetFont(L"PT Noeul");
  _selectTxt->SetFontSize(21.0f);
  _selectTxt->SetFontWeight(FontWeight::NORMAL);
  _selectTxt->SetColor({0.302, 0.024, 0.08, 1});
  _selectTxt->SetText(L"(선택)");
  _selectTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _selectTxt->SetDebugDraw(false);
#endif // _DEBUG
}
