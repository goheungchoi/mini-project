#include "SubMission.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"

SubMission::SubMission(World* world) : UIPanel(world)
{
  // SubMisson
  _subMissionImg = CreateUI<UIImage>(L"SubMissonImg");
  _subMissionImg->SetSprite("2D\\UI\\UI_Stage_S.png", {208, 240});
}

SubMission::~SubMission() {}

void SubMission::SetText(std::wstring text)
{
  _subMissionTxt = CreateUI<UIText>(L"SubMissionImg");
  _subMissionTxt->SetCenterPos({288, 290});
  _subMissionTxt->SetSize(
      {_subMissionImg->GetSize().x, _subMissionImg->GetSize().y});
  _subMissionTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _subMissionTxt->SetFont(L"PT Noeul");
  _subMissionTxt->SetFontSize(21.0f);
  _subMissionTxt->SetFontWeight(FontWeight::NORMAL);
  _subMissionTxt->SetText(text);
  _subMissionTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _subMissionTxt->SetDebugDraw(false);
#endif // _DEBUG
}
