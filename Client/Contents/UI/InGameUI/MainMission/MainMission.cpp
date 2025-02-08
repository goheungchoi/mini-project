#include "MainMission.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"

MainMission::MainMission(World* world) : UIPanel(world)
{
  _mainMissionImg = CreateUI<UIImage>(L"MainMissonImg");
  _mainMissionImg->SetSprite("2D\\UI\\UI_Stage_L.png", {338, 100});
}

MainMission::~MainMission() {}

void MainMission::SetText(std::wstring text)
{
  _mainMissionTxt = CreateUI<UIText>(L"MainMissonTxt");
  _mainMissionTxt->SetCenterPos({300, 130});
  _mainMissionTxt->SetSize(
      {_mainMissionImg->GetSize().x, _mainMissionImg->GetSize().y});
  _mainMissionTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _mainMissionTxt->SetFont(L"PT Noeul");
  _mainMissionTxt->SetFontSize(30.0f);
  _mainMissionTxt->SetFontWeight(FontWeight::NORMAL);
  _mainMissionTxt->SetText(text);
  _mainMissionTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _mainMissionTxt->SetDebugDraw(false);
#endif // _DEBUG

}
