#include "MainMission.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"

MainMission::MainMission(World* world) : UIPanel(world)
{
  _mainMissionImg = CreateUI<UIImage>(L"MainMissonImg");
  _mainMissionImg->SetSprite("2D\\UI\\UI_Stage_L.png", {338, 100});

  _mainMissionTxt = CreateUI<UIText>(L"MainMissonTxt");
  _mainMissionTxt->SetCenterPos({300, 130});
  _mainMissionTxt->SetSize(
      {_mainMissionImg->GetSize().x, _mainMissionImg->GetSize().y});
  _mainMissionTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _mainMissionTxt->SetFont(L"GangwonEduAll");
  _mainMissionTxt->SetFontWeight(FontWeight::BOLD);
  _mainMissionTxt->SetText(
      L"사라진 아이들에 대한 단서 찾기\n모든 적 처치 (0/8)");
  _mainMissionTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _mainMissionTxt->SetDebugDraw(false);
#endif // _DEBUG
}

MainMission::~MainMission() {}
