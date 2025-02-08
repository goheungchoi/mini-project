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

void SubMission::SetText(std::wstring text, Vector2 pos)
{
  _subMissionTxt = CreateUI<UIText>(L"SubMissionTxt");
  _subMissionTxt->SetCenterPos({pos.x + 60, pos.y});
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

  _selectTxt = CreateUI<UIText>(L"SubSelectTxt");
  _selectTxt->SetCenterPos(pos);
  _selectTxt->SetSize(
      {_subMissionImg->GetSize().x, _subMissionImg->GetSize().y});
  _selectTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _selectTxt->SetFont(L"PT Noeul");
  _selectTxt->SetFontSize(21.0f);
  _selectTxt->SetFontWeight(FontWeight::NORMAL);
  _selectTxt->SetColor({0.302, 0.024, 0.08, 1});
  _selectTxt->SetText(L"(¼±ÅÃ)");
  _selectTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _selectTxt->SetDebugDraw(false);
#endif // _DEBUG
}
