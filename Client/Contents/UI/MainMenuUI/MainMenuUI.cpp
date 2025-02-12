#include "MainMenuUI.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIImage/UIImage.h"

#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"

MainMenuUI::MainMenuUI(World* world) : UIPanel(world)
{
  _backGroundImg = CreateUI<UIImage>(L"BackGoundIMG");
  _backGroundImg->SetSprite("2D\\UI\\Title\\Title_Background.png", {960, 540});

  SoundManager::PlaySound(SoundList::Background_Title);


  //_LogoImage = CreateUI<UIImage>(L"TitleIMG");
  //_LogoImage->SetSprite("Textures\\TitleLogo.png", {556, 362});
  //_hiddenLogoImage = CreateUI<UIImage>(L"hiddenTitleIMG");
  //_hiddenLogoImage->SetSprite("Textures\\StartBtn.png", {556, 362});
  //_hiddenLogoImage->SetOpacity(0.f);
  //SetOnActivatedEvent([=]() {
  //  if (ending1)
  //  {
  //    _LogoImage->FadeOut(5.f);
  //    _hiddenLogoImage->FadeIn(5.f);
  //  }
  //});

  // Start Button
  _startImg[0] = CreateUI<UIImage>(L"StartIMG_Default");
  _startImg[1] = CreateUI<UIImage>(L"StartIMG_Hover");
  _startImg[0]->SetSprite("2D\\UI\\Title\\Title_Button_01_Basic.png", _btnPos);
  _startImg[1]->SetSprite("2D\\UI\\Title\\Title_Button_01_Hover.png", _btnPos);
  _startImg[1]->SetStatus(EStatus::EStatus_Inactive);

  _startBtn = CreateUI<UIButton>(L"StartButton");
  _startBtn->SetSize(_startImg[0]->GetSize());
  _startBtn->SetCenterPos(_btnPos);


#ifdef _DEBUG
  _startBtn->SetDebugDraw(false);
#endif // _DEBUG

  _startBtn->AddOnUnHoveredHandler([this]() {
    _bHoverBtn = false;
      _startImg[0]->SetStatus(EStatus::EStatus_Active);
      _startImg[1]->SetStatus(EStatus::EStatus_Inactive);
  });
  _startBtn->AddOnHoveredHandler([this]() {
    if (!_bHoverBtn)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHoverBtn = true;
    }
      _startImg[0]->SetStatus(EStatus::EStatus_Inactive);
      _startImg[1]->SetStatus(EStatus::EStatus_Active);
  });

  _startBtn->AddOnPressedHandler([this]() {
      _startImg[0]->SetStatus(EStatus::EStatus_Inactive);
      _startImg[1]->SetStatus(EStatus::EStatus_Active);
  });

  _startBtn->AddOnClickHandler([this]() {

    SoundManager::PlaySound(SoundList::Button_Click);
    SoundManager::StopSound(SoundList::Background_Title);
    _world->PrepareChangeLevel("Level1");
    _world->CommitLevelChange();
  });


  // Challenge Button
  _challengeImg[0] = CreateUI<UIImage>(L"ChallengeIMG_Default");
  _challengeImg[1] = CreateUI<UIImage>(L"ChallengeIMG_Hover");
  _challengeImg[0]->SetSprite("2D\\UI\\Title\\Title_Button_02_Basic.png",
                              {_btnPos.x, _btnPos.y + _interval});
  _challengeImg[1]->SetSprite("2D\\UI\\Title\\Title_Button_02_Hover.png",
                              {_btnPos.x, _btnPos.y + _interval});
  _challengeImg[1]->SetStatus(EStatus::EStatus_Inactive);

  _challengeBtn = CreateUI<UIButton>(L"ChallengeButton");
  _challengeBtn->SetSize(_challengeImg[0]->GetSize());
  _challengeBtn->SetCenterPos({_btnPos.x, _btnPos.y + _interval});


#ifdef _DEBUG
  _challengeBtn->SetDebugDraw(false);
#endif // _DEBUG

  _challengeBtn->AddOnUnHoveredHandler([this]() {
    _bHoverBtn = false;
    _challengeImg[0]->SetStatus(EStatus::EStatus_Active);
    _challengeImg[1]->SetStatus(EStatus::EStatus_Inactive);
  });
  _challengeBtn->AddOnHoveredHandler([this]() {
    if (!_bHoverBtn)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHoverBtn = true;
    }
    _challengeImg[0]->SetStatus(EStatus::EStatus_Inactive);
    _challengeImg[1]->SetStatus(EStatus::EStatus_Active);
  });

  _challengeBtn->AddOnPressedHandler([this]() {
    _challengeImg[0]->SetStatus(EStatus::EStatus_Inactive);
    _challengeImg[1]->SetStatus(EStatus::EStatus_Active);
  });

  _challengeBtn->AddOnClickHandler([this]() {
      
    SoundManager::StopSound(SoundList::Background_Title);
    _world->PrepareChangeLevel("Challenge Level");
    _world->CommitLevelChange();
  });


  // Exit Button
  _exitImg[0] = CreateUI<UIImage>(L"ChallengeIMG_Default");
  _exitImg[1] = CreateUI<UIImage>(L"ChallengeIMG_Hover");
  _exitImg[0]->SetSprite("2D\\UI\\Title\\Title_Button_03_Basic.png",
                         {_btnPos.x, _btnPos.y + _interval * 2});
  _exitImg[1]->SetSprite("2D\\UI\\Title\\Title_Button_03_Hover.png",
                         {_btnPos.x, _btnPos.y + _interval * 2});
  _exitImg[1]->SetStatus(EStatus::EStatus_Inactive);

  _exitBtn = CreateUI<UIButton>(L"ExitButton");
  _exitBtn->SetSize(_exitImg[0]->GetSize());
  _exitBtn->SetCenterPos({_btnPos.x, _btnPos.y + _interval * 2});

  #ifdef _DEBUG
  _exitBtn->SetDebugDraw(false);
#endif // _DEBUG

  _exitBtn->AddOnUnHoveredHandler([this]() {
    _bHoverBtn = false;
    _exitImg[0]->SetStatus(EStatus::EStatus_Active);
    _exitImg[1]->SetStatus(EStatus::EStatus_Inactive);
  });
  _exitBtn->AddOnHoveredHandler([this]() {
    if (!_bHoverBtn)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHoverBtn = true;
    }
    _exitImg[0]->SetStatus(EStatus::EStatus_Inactive);
    _exitImg[1]->SetStatus(EStatus::EStatus_Active);
  });

  _exitBtn->AddOnPressedHandler([this]() {
    _exitImg[0]->SetStatus(EStatus::EStatus_Inactive);
    _exitImg[1]->SetStatus(EStatus::EStatus_Active);
  });

  _exitBtn->AddOnClickHandler([this]() {
    SoundManager::StopSound(SoundList::Background_Title);
      PostQuitMessage(0); });

  // Cursor
  _cursor = CreateUI<UICursor>(L"Cursor");
  _cursor->SetCursorType(CursorType::DEFAULT);
}

MainMenuUI::~MainMenuUI() {}

void MainMenuUI::SetEndingFlag(int i)
{
  if (i == 1)
    ending1 = true;
  if (i == 2)
    ending2 = true;
}
