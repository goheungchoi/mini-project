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
  //1
  {
    _stage1Acitve = CreateUI<UIImage>(L"stage10_1");
    _stage1DeAcitve = CreateUI<UIImage>(L"BrawlerHoveredImage");
    _stage1Button = CreateUI<UIButton>(L"BrawlerButton");
    _stage1Acitve->SetSprite("2D\\UI\\StageNum\\StageNum_Hover_01.png",
                             {360, 360});
    _stage1DeAcitve->SetSprite("2D\\UI\\StageNum\\StageNum_Act_01.png",
                               {360, 360});
    _stage1Acitve->SetOpacity(0.f);
    _stage1DeAcitve->SetOpacity(1.f);
    _stage1Button->SetSize(_stage1Acitve->GetTextureSize());
    _stage1Button->SetCenterPos({360, 360});
  }
  //2
  {
    _stage2Acitve = CreateUI<UIImage>(L"stage10_2");
    _stage2DeAcitve = CreateUI<UIImage>(L"BrawlerHoveredImage");
    _stage2Button = CreateUI<UIButton>(L"BrawlerButton");
    _stage2Acitve->SetSprite("2D\\UI\\StageNum\\StageNum_Hover_02.png",
                             {760, 360});
    _stage2DeAcitve->SetSprite("2D\\UI\\StageNum\\StageNum_Act_02.png",
                               {760,360});
    _stage2Acitve->SetOpacity(0.f);
    _stage2DeAcitve->SetOpacity(1.f);
    _stage2Button->SetSize(_stage2Acitve->GetTextureSize());
    _stage2Button->SetCenterPos({760, 360});
  }
  //3
  {
    _stage3Acitve = CreateUI<UIImage>(L"stage10_3");
    _stage3DeAcitve = CreateUI<UIImage>(L"BrawlerHoveredImage");
    _stage3Button = CreateUI<UIButton>(L"BrawlerButton");
    _stage3Acitve->SetSprite("2D\\UI\\StageNum\\StageNum_Hover_03.png",
                             {1160, 360});
    _stage3DeAcitve->SetSprite("2D\\UI\\StageNum\\StageNum_Act_03.png",
                               {1160, 360});
    _stage3Acitve->SetOpacity(0.f);
    _stage3DeAcitve->SetOpacity(1.f);
    _stage3Button->SetSize(_stage3Acitve->GetTextureSize());
    _stage3Button->SetCenterPos({1160, 360});
  }
  //4
  {
    _stage4Acitve = CreateUI<UIImage>(L"stage10_4");
    _stage4DeAcitve = CreateUI<UIImage>(L"BrawlerHoveredImage");
    _stage4Button = CreateUI<UIButton>(L"BrawlerButton");
    _stage4Acitve->SetSprite("2D\\UI\\StageNum\\StageNum_Hover_04.png",
                             {1560, 360});
    _stage4DeAcitve->SetSprite("2D\\UI\\StageNum\\StageNum_Act_04.png",
                               {1560, 360});
    _stage4Acitve->SetOpacity(0.f);
    _stage4DeAcitve->SetOpacity(1.f);
    _stage4Button->SetSize(_stage4Acitve->GetTextureSize());
    _stage4Button->SetCenterPos({1560, 360});
  }
  //5
  {
    _stage5Acitve = CreateUI<UIImage>(L"stage10_5");
    _stage5DeAcitve = CreateUI<UIImage>(L"BrawlerHoveredImage");
    _stage5Button = CreateUI<UIButton>(L"BrawlerButton");
    _stage5Acitve->SetSprite("2D\\UI\\StageNum\\StageNum_Hover_05.png",
                             {560, 650});
    _stage5DeAcitve->SetSprite("2D\\UI\\StageNum\\StageNum_Act_05.png",
                               {560, 650});
    _stage5Acitve->SetOpacity(0.f);
    _stage5DeAcitve->SetOpacity(1.f);
    _stage5Button->SetSize(_stage5Acitve->GetTextureSize());
    _stage5Button->SetCenterPos({560, 650});
  }
  // 6
  {
    _stage6Acitve = CreateUI<UIImage>(L"stage10_6");
    _stage6DeAcitve = CreateUI<UIImage>(L"BrawlerHoveredImage");
    _stage6Button = CreateUI<UIButton>(L"BrawlerButton");
    _stage6Acitve->SetSprite("2D\\UI\\StageNum\\StageNum_Hover_06.png",
                             {960, 650});
    _stage6DeAcitve->SetSprite("2D\\UI\\StageNum\\StageNum_Act_06.png",
                               {960, 650});
    _stage6Acitve->SetOpacity(0.f);
    _stage6DeAcitve->SetOpacity(1.f);
    _stage6Button->SetSize(_stage6Acitve->GetTextureSize());
    _stage6Button->SetCenterPos({960, 650});
  }
  // 7
  {
    _stage7Acitve = CreateUI<UIImage>(L"stage10_7");
    _stage7DeAcitve = CreateUI<UIImage>(L"BrawlerHoveredImage");
    _stage7Button = CreateUI<UIButton>(L"BrawlerButton");
    _stage7Acitve->SetSprite("2D\\UI\\StageNum\\StageNum_Hover_07.png",
                             {1360, 650});
    _stage7DeAcitve->SetSprite("2D\\UI\\StageNum\\StageNum_Act_07.png",
                               {1360, 650});
    _stage7Acitve->SetOpacity(0.f);
    _stage7DeAcitve->SetOpacity(1.f);
    _stage7Button->SetSize(_stage7Acitve->GetTextureSize());
    _stage7Button->SetCenterPos({1360, 650});
  }

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
      transitionUI->FadeOut(3.0f);
    }
  });


  _stage2Button->AddOnHoveredHandler([=]() {
    _stage2Acitve->SetOpacity(1.f);
    _stage2DeAcitve->SetOpacity(0.f);
  });
  _stage2Button->AddOnUnHoveredHandler([=]() {
    _stage2Acitve->SetOpacity(0.f);
    _stage2DeAcitve->SetOpacity(1.f);
  });
  _stage2Button->AddOnClickHandler([=]() {
    _world->PrepareChangeLevel("Level10_2");
    TransitionUI* transitionUI =
        _world->_canvas->GetPanel<TransitionUI>(L"FadeTransition");
    if (transitionUI)
    {
      transitionUI->FadeOut(3.0f);
    }
  });

    _stage3Button->AddOnHoveredHandler([=]() {
    _stage3Acitve->SetOpacity(1.f);
    _stage3DeAcitve->SetOpacity(0.f);
  });
  _stage3Button->AddOnUnHoveredHandler([=]() {
    _stage3Acitve->SetOpacity(0.f);
    _stage3DeAcitve->SetOpacity(1.f);
  });
  _stage3Button->AddOnClickHandler([=]() {
    _world->PrepareChangeLevel("Level10_3");
    TransitionUI* transitionUI =
        _world->_canvas->GetPanel<TransitionUI>(L"FadeTransition");
    if (transitionUI)
    {
      transitionUI->FadeOut(3.0f);
    }
  });

    _stage4Button->AddOnHoveredHandler([=]() {
    _stage4Acitve->SetOpacity(1.f);
    _stage4DeAcitve->SetOpacity(0.f);
  });
  _stage4Button->AddOnUnHoveredHandler([=]() {
    _stage4Acitve->SetOpacity(0.f);
    _stage4DeAcitve->SetOpacity(1.f);
  });
  _stage4Button->AddOnClickHandler([=]() {
    _world->PrepareChangeLevel("Level10_4");
    TransitionUI* transitionUI =
        _world->_canvas->GetPanel<TransitionUI>(L"FadeTransition");
    if (transitionUI)
    {
      transitionUI->FadeOut(3.0f);
    }
  });

  _stage5Button->AddOnHoveredHandler([=]() {
    _stage5Acitve->SetOpacity(1.f);
    _stage5DeAcitve->SetOpacity(0.f);
  });
  _stage5Button->AddOnUnHoveredHandler([=]() {
    _stage5Acitve->SetOpacity(0.f);
    _stage5DeAcitve->SetOpacity(1.f);
  });
  _stage5Button->AddOnClickHandler([=]() {
    _world->PrepareChangeLevel("Level10_5");
    TransitionUI* transitionUI =
        _world->_canvas->GetPanel<TransitionUI>(L"FadeTransition");
    if (transitionUI)
    {
      transitionUI->FadeOut(3.0f);
    }
  });

  _stage6Button->AddOnHoveredHandler([=]() {
    _stage6Acitve->SetOpacity(1.f);
    _stage6DeAcitve->SetOpacity(0.f);
  });
  _stage6Button->AddOnUnHoveredHandler([=]() {
    _stage6Acitve->SetOpacity(0.f);
    _stage6DeAcitve->SetOpacity(1.f);
  });
  _stage6Button->AddOnClickHandler([=]() {
    _world->PrepareChangeLevel("Level10_6");
    TransitionUI* transitionUI =
        _world->_canvas->GetPanel<TransitionUI>(L"FadeTransition");
    if (transitionUI)
    {
      transitionUI->FadeOut(3.0f);
    }
  });

  _stage7Button->AddOnHoveredHandler([=]() {
    _stage7Acitve->SetOpacity(1.f);
    _stage7DeAcitve->SetOpacity(0.f);
  });
  _stage7Button->AddOnUnHoveredHandler([=]() {
    _stage7Acitve->SetOpacity(0.f);
    _stage7DeAcitve->SetOpacity(1.f);
  });
  _stage7Button->AddOnClickHandler([=]() {
    _world->PrepareChangeLevel("Level10_7");
    TransitionUI* transitionUI =
        _world->_canvas->GetPanel<TransitionUI>(L"FadeTransition");
    if (transitionUI)
    {
      transitionUI->FadeOut(3.0f);
    }
  });
}

  void ChallengeModeUI::Update(float dt)
{
  UIPanel::Update(dt);

  //_backGroundImg->SetOpacity(1.f);
}

