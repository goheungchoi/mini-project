#include "MainMenuUI.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"

MainMenuUI::MainMenuUI(World* world) : UIPanel(world)
{
  _backGroundImage = CreateUI<UIImage>(L"BackGoundIMG");
  _backGroundImage->SetSprite("Textures\\TitleBackGround.png", {0,0});

  _LogoImage = CreateUI<UIImage>(L"BackGoundIMG");
  _LogoImage->SetSprite("Textures\\TitleLogo.png", {100, 200});

  // Start Button
  _startBtn = CreateUI<UIButton>(L"StartButton");

  _startImage = CreateUI<UIImage>(L"StartBtnImage");
  _startImage->SetSprite("Textures\\Test_QuestUI_1.png", {270, 580});
  _startBtn->SetSize(_startImage->GetSize());
}

MainMenuUI::~MainMenuUI() {}
