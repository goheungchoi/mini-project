#include "MainMission.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"

MainMission::MainMission(World* world) : UIPanel(world)
{
  _mainMissionImg = CreateUI<UIImage>(L"MainMissonImg");
  _mainMissionImg->SetSprite("2D\\UI\\UI_Stage_L.png", {338, 100});
}

MainMission::~MainMission() {}

void MainMission::SetText(std::wstring text, Vector2 pos)
{
  _mainMissionTxt = CreateUI<UIText>(L"MainMissonTxt");
  _mainMissionTxt->SetCenterPos({pos.x + 85, pos.y});
  _mainMissionTxt->SetSize(
      {_mainMissionImg->GetSize().x, _mainMissionImg->GetSize().y});
  _mainMissionTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _mainMissionTxt->SetFont(L"PT Noeul");
  _mainMissionTxt->SetFontSize(30.0f);
  _mainMissionTxt->SetFontWeight(FontWeight::NORMAL);
  _mainMissionTxt->SetText(text.c_str());
  _mainMissionTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _mainMissionTxt->SetDebugDraw(false);
#endif // _DEBUG


    _selectTxt = CreateUI<UIText>(L"MainSelectTxt");
  _selectTxt->SetCenterPos(pos);
  _selectTxt->SetSize(
      {_mainMissionImg->GetSize().x, _mainMissionImg->GetSize().y});
  _selectTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _selectTxt->SetFont(L"PT Noeul");
  _selectTxt->SetFontSize(30.0f);
  _selectTxt->SetFontWeight(FontWeight::NORMAL);
  _selectTxt->SetColor({0.75, 0.1, 0.12, 1});
  _selectTxt->SetText(L"(선택)");
  _selectTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _selectTxt->SetDebugDraw(false);
#endif // _DEBUG
}
