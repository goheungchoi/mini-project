#include "InGameUI.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"

InGameUI::InGameUI(World* world) : UIPanel(world)
{
  // MainMisson
  _mainMissonImg = CreateUI<UIImage>(L"MainMissonImg");
  _mainMissonImg->SetSprite("Textures\\MainMissonUI_Test.png", {338, 100});
  _mainMissonTxt = CreateUI<UIText>(L"MainMissonTxt");
  _mainMissonTxt->SetCenterPos({200, 100});
  _mainMissonTxt->SetText(L"¹Ì¼ÇTxt1234");
  _mainMissonTxt->SetOpacity(0.5f);
#ifdef _DEBUG
  _mainMissonTxt->SetDebugDraw(true);
#endif // _DEBUG

  // SubMisson
  _subMissonImg = CreateUI<UIImage>(L"MainMissonImg");
  _subMissonImg->SetSprite("Textures\\SubMissonUI_Test.png", {208, 240});

  // SnipingBtn
  _snipingBtnImg = CreateUI<UIImage>(L"SnipingBtnImg");
  _snipingBtnImg->SetSprite("Textures\\SnipingUI_Test.png", {1730, 720});

  // CombatBtn
  _combatBtnImg = CreateUI<UIImage>(L"CombatBtnImg");
  _combatBtnImg->SetSprite("Textures\\CombatUI_Test.png", {1600, 960});
}

InGameUI::~InGameUI() {}
