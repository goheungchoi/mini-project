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
  Vector2 textPos = {_pos.x-20, _pos.y + 27};

  _mainMissionTxt = CreateUI<UIText>(L"MainMissonTxt");
  _mainMissionTxt->SetCenterPos({textPos});
  _mainMissionTxt->SetSize(
      {_mainMissionImg->GetSize().x, _mainMissionImg->GetSize().y});
  _mainMissionTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _mainMissionTxt->SetFont(L"PT Noeul");
  _mainMissionTxt->SetFontSize(40.0f);
  _mainMissionTxt->SetFontWeight(FontWeight::NORMAL);
  _mainMissionTxt->SetText(text.c_str());
  _mainMissionTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _mainMissionTxt->SetDebugDraw(false);
#endif // _DEBUG


//    _selectTxt = CreateUI<UIText>(L"MainSelectTxt");
//  _selectTxt->SetCenterPos({textPos.x - 85, textPos.y});
//  _selectTxt->SetSize(
//      {_mainMissionImg->GetSize().x, _mainMissionImg->GetSize().y});
//  _selectTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
//  _selectTxt->SetFont(L"PT Noeul");
//  _selectTxt->SetFontSize(30.0f);
//  _selectTxt->SetFontWeight(FontWeight::NORMAL);
//  _selectTxt->SetColor({0.302, 0.024, 0.08, 1});
//  _selectTxt->SetText(L"(선택)");
//  _selectTxt->SetOpacity(1.0f);
//#ifdef _DEBUG
//  _selectTxt->SetDebugDraw(false);
//#endif // _DEBUG
}
