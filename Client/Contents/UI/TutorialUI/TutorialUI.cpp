#include "TutorialUI.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIButton/UIButton.h"

#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"

TutorialUI::TutorialUI(World* world) : UIPanel(world)
{
  // TutorialIMG
  _tutorialIMG[0] = CreateUI<UIImage>(L"TutorialIMG_01");
  _tutorialIMG[1] = CreateUI<UIImage>(L"TutorialIMG_02");
  _tutorialIMG[2] = CreateUI<UIImage>(L"TutorialIMG_03");
  _tutorialIMG[3] = CreateUI<UIImage>(L"TutorialIMG_04");

  _tutorialIMG[0]->SetSprite("2D\\UI\\Tuto\\PopupUI_01.png", {960, 540});
  _tutorialIMG[1]->SetSprite("2D\\UI\\Tuto\\PopupUI_02.png", {960, 540});
  _tutorialIMG[2]->SetSprite("2D\\UI\\Tuto\\PopupUI_03.png", {960, 540});
  _tutorialIMG[3]->SetSprite("2D\\UI\\Tuto\\PopupUI_04.png", {960, 540});
  
  _tutorialIMG[0]->SetOpacity(1.0f);
  _tutorialIMG[1]->SetOpacity(0.0f);
  _tutorialIMG[2]->SetOpacity(0.0f);
  _tutorialIMG[3]->SetOpacity(0.0f);


  // NextButton
  _nextIMG[0] = CreateUI<UIImage>(L"NextIMG_Default");
  _nextIMG[1] = CreateUI<UIImage>(L"NextIMG_Hover");

  _nextIMG[0]->SetSprite("2D\\UI\\Tuto\\PopupButton_default.png", _btnPos);
  _nextIMG[1]->SetSprite("2D\\UI\\Tuto\\PopupButton_hover.png", _btnPos);

  _nextIMG[0]->SetOpacity(1.0f);
  _nextIMG[1]->SetOpacity(0.0f);

  _nextBtn = CreateUI<UIButton>(L"NextButton");
  _nextBtn->SetSize(_nextIMG[0]->GetSize());
  _nextBtn->SetCenterPos(_btnPos);

#ifdef _DEBUG
  _nextBtn->SetDebugDraw(true);
#endif // _DEBUG

  _nextBtn->AddOnHoveredHandler([this]() {
    if (!_bHoverFlag)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHoverFlag = true;
    }
  });
  _nextBtn->AddOnUnHoveredHandler([this]() { _bHoverFlag = false; });
  _nextBtn->AddOnClickHandler([this]() {});


  // CloseButton
  _closeIMG[0] = CreateUI<UIImage>(L"CloseIMG_Default");
  _closeIMG[1] = CreateUI<UIImage>(L"CloseIMG_Hover");

  _closeIMG[0]->SetSprite("2D\\UI\\Tuto\\PopupButton2_default.png", _btnPos);
  _closeIMG[1]->SetSprite("2D\\UI\\Tuto\\PopupButton2_hover.png", _btnPos);

  _closeIMG[0]->SetOpacity(1.0f);
  _closeIMG[1]->SetOpacity(0.0f);

  _closeBtn = CreateUI<UIButton>(L"CloseButton");
  _closeBtn->SetSize(_closeIMG[0]->GetSize());
  _closeBtn->SetCenterPos({_btnPos.x + 100, _btnPos.y});

#ifdef _DEBUG
  _closeBtn->SetDebugDraw(true);
#endif // _DEBUG

  _closeBtn->AddOnHoveredHandler([this]() {
    if (!_bHoverFlag)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHoverFlag = true;
    }
      });
  _closeBtn->AddOnUnHoveredHandler([this]() { _bHoverFlag = false; });
  _closeBtn->AddOnClickHandler([this]() {});

}

TutorialUI::~TutorialUI() {}

void TutorialUI::Update(float dt) {}
