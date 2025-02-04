#include "MainMenuUI.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"

MainMenuUI::MainMenuUI(World* world) : UIPanel(world)
{
  _backGroundImage = CreateUI<UIImage>(L"BackGoundIMG");
  _backGroundImage->SetSprite("Textures\\TitleBackGround.png", {0, 0});

  _LogoImage = CreateUI<UIImage>(L"TitleIMG");
  _LogoImage->SetSprite("Textures\\TitleLogo.png", {100, 200});

  // Start Button
  _startBtn = CreateUI<UIButton>(L"StartButton");
  _startBtn->SetPosition({270, 580});
  _startBtn->AddOnHoveredHandler([this]() {
    _startImage->SetSprite("Textures\\StartBtn_hover.png", {260, 580});
  });
  _startBtn->AddOnUnHoveredHandler(
      [this]() { _startImage->SetStatus(EStatus::EStatus_Inactive); });
  _startBtn->AddOnUnHoveredHandler(
      [this]() { _backGroundImage->SetStatus(EStatus::EStatus_Inactive); });
  _startBtn->AddOnUnHoveredHandler(
      [this]() { _LogoImage->SetStatus(EStatus::EStatus_Inactive); });

  _startImage = CreateUI<UIImage>(L"StartBtnImage");
  _startImage->SetSprite("Textures\\StartBtn.png", {270, 580});
  _startBtn->SetSize(_startImage->GetSize());
}

MainMenuUI::~MainMenuUI() {}
