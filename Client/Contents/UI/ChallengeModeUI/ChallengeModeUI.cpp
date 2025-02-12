#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "ChallengeModeUI.h"
#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"
#include "GameFramework/World/World.h"
#include "Contents/UI/TransitionUI/TransitionUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
ChallengeModeUI::ChallengeModeUI(World* world) : UIPanel(world)
{
  _world = world;
  _backGroundImg = CreateUI<UIImage>(L"vignette");
  _backGroundImg->SetSprite("2D\\UI\\StageNum\\ChallengeBG.png", {960, 540});
  _backGroundImg->SetOpacity(1.f);
  _stage1Acitve = CreateUI<UIImage>(L"stage10_1");
  _stage1DeAcitve = CreateUI<UIImage>(L"BrawlerHoveredImage");
  _stage1Button = CreateUI<UIButton>(L"BrawlerButton");
  _stage1Acitve->SetSprite("2D\\UI\\StageNum\\StageNum_Hover_01.png",
                           {350, 350});
  _stage1DeAcitve->SetSprite(
      "2D\\UI\\StageNum\\StageNum_Act_01.png", {350, 350});
  _stage1Acitve->SetOpacity(0.f);
  _stage1DeAcitve->SetOpacity(1.f);
  _stage1Button->SetSize(_stage1Acitve->GetTextureSize());
  _stage1Button->SetCenterPos({350, 350});


  // Cursor
  _cursor = CreateUI<UICursor>(L"Cursor");
  _cursor->SetCursorType(CursorType::DEFAULT);
}

ChallengeModeUI::~ChallengeModeUI()
{
  
}

void ChallengeModeUI::BeginLevel() 
{
  UIPanel::BeginLevel();
  _stage1Button->AddOnHoveredHandler([=]() {
    _stage1Acitve->SetOpacity(1.f);
    _stage1DeAcitve->SetOpacity(0.f);
  });
  _stage1Button->AddOnUnHoveredHandler([=]() {
    _stage1Acitve->SetOpacity(0.f);
    _stage1DeAcitve->SetOpacity(1.f);
  });
  _stage1Button->AddOnClickHandler([=]() {
    _world->PrepareChangeLevel("Level10_1");
    TransitionUI* transitionUI =
        _world->_canvas->GetPanel<TransitionUI>(L"FadeTransition");
    if (transitionUI)
    {
      transitionUI->FadeOut(3.5f);
    }
  });
}

  void ChallengeModeUI::Update(float dt)
{
  UIPanel::Update(dt);

  //_backGroundImg->SetOpacity(1.f);
}

