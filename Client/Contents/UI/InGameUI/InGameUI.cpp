#include "InGameUI.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"
//#include "Core/Types/FontType.h"

InGameUI::InGameUI(World* world) : UIPanel(world)
{
  // MainMisson
  
  _mainMissonImg = CreateUI<UIImage>(L"MainMissonImg");
  _mainMissonImg->SetSprite("Textures\\MainMissonUI_Test.png", {338, 100});
  _mainMissonTxt = CreateUI<UIText>(L"MainMissonTxt");
  _mainMissonTxt->SetCenterPos({300, 130});
  _mainMissonTxt->SetSize(
      {_mainMissonImg->GetSize().x, _mainMissonImg->GetSize().y});
  _mainMissonTxt->SetTextAlignment(TextAlignment::LEFTAlIGN);
  _mainMissonTxt->SetFont(L"HY견고딕");
  _mainMissonTxt->SetText(L"사라진 아이들에 대한 단서 찾기\n모든 적 처치 (0/8)");
  _mainMissonTxt->SetOpacity(1.0f);
#ifdef _DEBUG
  _mainMissonTxt->SetDebugDraw(false);
#endif // _DEBUG

  // SubMisson
  _subMissonImg = CreateUI<UIImage>(L"MainMissonImg");
  _subMissonImg->SetSprite("Textures\\SubMissonUI_Test.png", {208, 240});

  // SnipingBtn
  _snipingBtnImg = CreateUI<UIImage>(L"SnipingBtnImg");
  _snipingBtnImg->SetSprite("Textures\\SnipingUI_Test.png", {1770, 720});

  // CombatBtn
  _combatBtnImg = CreateUI<UIImage>(L"CombatBtnImg");
  _combatBtnImg->SetSprite("Textures\\Btn_Slot.png", {1600, 960});

  // PlayBtn
  _playBtn = CreateUI<UIButton>(L"PlayBtn");
  _playBtnImg = CreateUI<UIImage>(L"PlayBtnImg");
  _playBtnImg->SetSprite("Textures\\Btn_Play.png", {1800, 100});
     // Map::TriggerAction() 기능을 어떻게 가져와야 할까????
}

InGameUI::~InGameUI() {}
