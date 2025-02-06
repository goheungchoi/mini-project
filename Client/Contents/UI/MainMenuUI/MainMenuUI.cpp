#include "MainMenuUI.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"

MainMenuUI::MainMenuUI(World* world) : UIPanel(world)
{

  _backGroundImage = CreateUI<UIImage>(L"BackGoundIMG");
  _backGroundImage->SetSprite("Textures\\TitleBackGround.png", {960, 540});

  _LogoImage = CreateUI<UIImage>(L"TitleIMG");
  _LogoImage->SetSprite("Textures\\TitleLogo.png", {556, 362});

  _startBtnImage = CreateUI<UIImage>(L"StartBtnImage");
  _startBtnImage->SetSprite("Textures\\StartBtn.png", {573, 663});

  _startBtnHoverImage = CreateUI<UIImage>(L"StartBtnHoverImage");
  _startBtnHoverImage->SetSprite("Textures\\StartBtn_hover.png", {573, 663});
  _startBtnHoverImage->SetStatus(EStatus::EStatus_Inactive);

  _startBtnPressedImage = CreateUI<UIImage>(L"StartBtnPressedImage");
  _startBtnPressedImage->SetSprite("Textures\\StartBtn_click.png", {573, 663});
  _startBtnPressedImage->SetStatus(EStatus::EStatus_Inactive);

  // Start Button
  _startBtn = CreateUI<UIButton>(L"StartButton");
  _startBtn->SetSize(_startBtnImage->GetSize());
  _startBtn->SetCenterPos({573, 663});

#ifdef _DEBUG
  _startBtn->SetDebugDraw(true);
#endif // _DEBUG

  _startBtn->AddOnUnHoveredHandler([this]() {
    if (_LogoImage->GetStatus() == EStatus::EStatus_Active)
    {
      _startBtnHoverImage->SetStatus(EStatus::EStatus_Inactive);
      _startBtnPressedImage->SetStatus(EStatus::EStatus_Inactive);
      _startBtnImage->SetStatus(EStatus::EStatus_Active);
    }
  });
  _startBtn->AddOnHoveredHandler([this]() {
    if (_LogoImage->GetStatus() == EStatus::EStatus_Active)
    {
      _startBtnImage->SetStatus(EStatus::EStatus_Inactive);
      _startBtnPressedImage->SetStatus(EStatus::EStatus_Inactive);
      _startBtnHoverImage->SetStatus(EStatus::EStatus_Active);
    }
  });

  _startBtn->AddOnPressedHandler([this]() {
    if (_LogoImage->GetStatus() == EStatus::EStatus_Active)
    {
      _startBtnImage->SetStatus(EStatus::EStatus_Inactive);
      _startBtnHoverImage->SetStatus(EStatus::EStatus_Inactive);
      _startBtnPressedImage->SetStatus(EStatus::EStatus_Active);
    }
  });

  _startBtn->AddOnClickHandler([this]() {
    _world->PrepareChangeLevel("Level1");
    _world->CommitLevelChange();
  });

  //_startBtn->AddOnUnHoveredHandler(
  //    [this]() { _backGroundImage->SetStatus(EStatus::EStatus_Inactive); });

  _cursor = CreateUI<UICursor>(L"Cursor");
  _cursor->SetCursorType(CursorType::DEFAULT);
}

MainMenuUI::~MainMenuUI() {}
