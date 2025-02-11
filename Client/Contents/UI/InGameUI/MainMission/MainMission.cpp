#include "MainMission.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"

MainMission::MainMission(World* world) : UIPanel(world)
{
  _mainMissionImg = CreateUI<UIImage>(L"MainMissonImg");
  _mainMissionImg->SetSprite("2D\\UI\\UI_Stage_L.png", _pos);
}

MainMission::~MainMission() {}

void MainMission::SetText(std::wstring text)
{
  Vector2 textPos = {_pos.x-20, _pos.y + 12};

  _mainMissionTxt = CreateUI<UIText>(L"MainMissonTxt");
  _mainMissionTxt->SetCenterPos({textPos});
  _mainMissionTxt->SetSize(
      {_mainMissionImg->GetSize().x, _mainMissionImg->GetSize().y});
  _mainMissionTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _mainMissionTxt->SetFont(L"PT Noeul");
  _mainMissionTxt->SetFontSize(37.0f);
  _mainMissionTxt->SetFontWeight(FontWeight::NORMAL);
  _mainMissionTxt->SetText(text.c_str());
  _mainMissionTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _mainMissionTxt->SetDebugDraw(false);
#endif // _DEBUG


  _defaultTxt = CreateUI<UIText>(L"MainSelectTxt");
  _defaultTxt->SetCenterPos({textPos.x, textPos.y + 40});
  _defaultTxt->SetSize(
      {_mainMissionImg->GetSize().x, _mainMissionImg->GetSize().y});
  _defaultTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _defaultTxt->SetFont(L"PT Noeul");
  _defaultTxt->SetFontSize(30.0f);
  _defaultTxt->SetFontWeight(FontWeight::NORMAL);
  _defaultTxt->SetText(L"모든 적 처치");
  _defaultTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _defaultTxt->SetDebugDraw(false);
#endif // _DEBUG
}
