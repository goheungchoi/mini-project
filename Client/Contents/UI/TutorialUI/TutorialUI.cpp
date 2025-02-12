#include "TutorialUI.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "Contents/UI/AudioDramaUI/AudioDramaUI.h"
#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"
#include "Contents/GameObjects/Map/Map.h"
#include "GameFramework/UI/Canvas/Canvas.h"

TutorialUI::TutorialUI(World* world) : UIPanel(world)
{
  _world = world;

  if (_map)
  {
    _map = _world->FindGameObjectByType<Map>();
  }

  // TutorialIMG
  _tutorialIMG[0] = CreateUI<UIImage>(L"TutorialIMG_01");
  _tutorialIMG[1] = CreateUI<UIImage>(L"TutorialIMG_02");
  _tutorialIMG[2] = CreateUI<UIImage>(L"TutorialIMG_03");
  _tutorialIMG[3] = CreateUI<UIImage>(L"TutorialIMG_04");

  _tutorialIMG[0]->SetSprite("2D\\UI\\Tuto\\PopupUI_01.png", {960, 540});
  _tutorialIMG[1]->SetSprite("2D\\UI\\Tuto\\PopupUI_02.png", {960, 540});
  _tutorialIMG[2]->SetSprite("2D\\UI\\Tuto\\PopupUI_03.png", {960, 540});
  _tutorialIMG[3]->SetSprite("2D\\UI\\Tuto\\PopupUI_04.png", {960, 540});
  
  _tutorialIMG[0]->Activate();
  _tutorialIMG[1]->Activate();
  _tutorialIMG[2]->Activate();
  _tutorialIMG[3]->Activate();

  _tutorialIMG[0]->SetLateRender(true);
  _tutorialIMG[1]->SetLateRender(true);
  _tutorialIMG[2]->SetLateRender(true);
  _tutorialIMG[3]->SetLateRender(true);

  _tutorialIMG[0]->SetOpacity(1.0f);
  _tutorialIMG[1]->SetOpacity(0.0f);
  _tutorialIMG[2]->SetOpacity(0.0f);
  _tutorialIMG[3]->SetOpacity(0.0f);


  // CloseButton
  _closeIMG[0] = CreateUI<UIImage>(L"CloseIMG_Default");
  _closeIMG[1] = CreateUI<UIImage>(L"CloseIMG_Hover");

  _closeIMG[0]->SetSprite("2D\\UI\\Tuto\\PopupButton2_default.png", _btnPos);
  _closeIMG[1]->SetSprite("2D\\UI\\Tuto\\PopupButton2_hover.png", _btnPos);

  _closeIMG[0]->Activate();
  _closeIMG[1]->Activate();

  _closeIMG[0]->SetLateRender(true);
  _closeIMG[1]->SetLateRender(true);

  _closeIMG[0]->SetOpacity(1.0f);
  _closeIMG[1]->SetOpacity(0.0f);

  _closeBtn = CreateUI<UIButton>(L"CloseButton");
  _closeBtn->SetSize(_closeIMG[0]->GetSize());
  _closeBtn->SetCenterPos(_btnPos);
  _closeBtn->Deactivate();

#ifdef _DEBUG
  _closeBtn->SetDebugDraw(true);
#endif // _DEBUG

  _closeBtn->AddOnHoveredHandler([this]() {
    _closeIMG[0]->SetOpacity(0.0f);
    _closeIMG[1]->SetOpacity(1.0f);
    if (!_bHoverFlag)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHoverFlag = true;
    }
  });

  _closeBtn->AddOnUnHoveredHandler([this]() {
    _bHoverFlag = false;
    _closeIMG[0]->SetOpacity(1.0f);
    _closeIMG[1]->SetOpacity(0.0f);
      });

  _closeBtn->AddOnClickHandler([this]() { Deactivate(); });


  // NextButton
  _nextIMG[0] = CreateUI<UIImage>(L"NextIMG_Default");
  _nextIMG[1] = CreateUI<UIImage>(L"NextIMG_Hover");

  _nextIMG[0]->SetSprite("2D\\UI\\Tuto\\PopupButton_default.png", _btnPos);
  _nextIMG[1]->SetSprite("2D\\UI\\Tuto\\PopupButton_hover.png", _btnPos);

  _nextIMG[0]->Activate();
  _nextIMG[1]->Activate();

  _nextIMG[0]->SetLateRender(true);
  _nextIMG[1]->SetLateRender(true);

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
    _nextIMG[0]->SetOpacity(0.0f);
    _nextIMG[1]->SetOpacity(1.0f);
  });

  _nextBtn->AddOnUnHoveredHandler([this]() {
    _bHoverFlag = false;
    _nextIMG[0]->SetOpacity(1.0f);
    _nextIMG[1]->SetOpacity(0.0f);
  });

  _nextBtn->AddOnClickHandler([this]() {
    if (curTutorialIdx < 3)
    {
      ++curTutorialIdx;
    }

    for (int i = 0; i < 4; ++i)
    {
      _tutorialIMG[i]->SetOpacity(i == curTutorialIdx ? 1.0f : 0.0f);
    }

    if (curTutorialIdx == 3)
    {
      _nextIMG[0]->SetOpacity(0.0f);
      _nextIMG[1]->SetOpacity(0.0f);
      _closeBtn->Activate();
    }
  });

}

TutorialUI::~TutorialUI() {}

void TutorialUI::Update(float dt)
{
  __super::Update(dt);


  for (auto& [name, UIPanel] : _world->_canvas->panelMap)
  {
    if (name == L"AudioDramaUI")
    {
      if (static_cast<AudioDramaUI*>(UIPanel)->PlayFlag)
      {
        _tutorialIMG[0]->SetOpacity(0.0f);
        _tutorialIMG[1]->SetOpacity(0.0f);
        _tutorialIMG[2]->SetOpacity(0.0f);
        _tutorialIMG[3]->SetOpacity(0.0f);

        _nextIMG[0]->SetOpacity(0.0f);
        _nextIMG[1]->SetOpacity(0.0f);

        _closeIMG[0]->SetOpacity(0.0f);
        _closeIMG[1]->SetOpacity(0.0f);
      }
      else
      {
        _tutorialIMG[0]->SetOpacity(1.0f);
        _closeIMG[0]->SetOpacity(1.0f);
        _nextIMG[0]->SetOpacity(1.0f);
      }
    }
  }


  if (curTutorialIdx >= 1)
  {
    _tutorialIMG[0]->SetOpacity(0.0f);
  }

  if (_tutorialIMG[0]->GetOpacity() == 0.0f)
  {
    _nextBtn->Deactivate();
  }
  
  if (_tutorialIMG[0]->GetOpacity() == 1.0f ||
      _tutorialIMG[1]->GetOpacity() == 1.0f ||
      _tutorialIMG[2]->GetOpacity() == 1.0f)
  {
    _nextBtn->Activate();
  }

  if (_tutorialIMG[3]->GetOpacity() == 1.0f)
  {
    _nextIMG[0]->SetOpacity(0.0f);
    _nextIMG[1]->SetOpacity(0.0f);
    _nextBtn->Deactivate();

    _closeBtn->Activate();
  }
}
