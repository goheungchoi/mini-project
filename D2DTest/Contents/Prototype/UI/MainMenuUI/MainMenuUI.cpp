#include "MainMenuUI.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"

MainMenuUI::MainMenuUI(World* world) : UIPanel(world)
{
  //_backGroundImage = CreateUI<UIImage>(L"BackGoundIMG");
  //_backGroundImage->SetSprite("Textures\\TitleBackGround.png", {960, 540});

  _LogoImage = CreateUI<UIImage>(L"TitleIMG");
  _LogoImage->SetSprite("Textures\\TitleLogo.png", {556, 362});

  _startBtnImage = CreateUI<UIImage>(L"StartBtnImage");
  _startBtnImage->SetSprite("Textures\\StartBtn.png", {573, 663});

  // Start Button
  _startBtn = CreateUI<UIButton>(L"StartButton");
  _startBtn->SetSize(_startBtnImage->GetSize());
  _startBtn->SetPosition({573, 663});

#ifdef _DEBUG
  _startBtn->SetDebugDraw(true);
#endif // _DEBUG

  _startBtn->AddOnHoveredHandler([this]() {
    if (_LogoImage->GetStatus() == EStatus::EStatus_Active)
    {
      _startBtnImage->SetSprite("Textures\\StartBtn_hover.png", {573, 663});
    }
  });

  //_startBtn->AddOnClickHandler([this]() {
  //  _backGroundImage->SetStatus(EStatus::EStatus_Inactive);
  //  _LogoImage->SetStatus(EStatus::EStatus_Inactive);
  //  _startBtnImage->SetStatus(EStatus::EStatus_Inactive);
  //});

  _startBtn->AddOnPressedHandler([this]() {
    _startBtnImage->SetSprite("Textures\\StartBtn_click.png", {573, 663});
  });

  _startBtn->AddOnUnHoveredHandler([this]() {
    if (_LogoImage->GetStatus() == EStatus::EStatus_Active)
    {
      _startBtnImage->SetSprite("Textures\\StartBtn.png", {573, 663});
    }
  });

  //_startBtn->AddOnUnHoveredHandler(
  //    [this]() { _backGroundImage->SetStatus(EStatus::EStatus_Inactive); });
  //_startBtn->AddOnUnHoveredHandler(
  //    [this]() { _LogoImage->SetStatus(EStatus::EStatus_Inactive); });
}

MainMenuUI::~MainMenuUI() {}
