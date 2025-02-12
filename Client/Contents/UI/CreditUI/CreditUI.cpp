#include "CreditUI.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UICursor/UICursor.h"


CreditUI::CreditUI(World* world) : UIPanel(world)
{
  _backIMG = CreateUI<UIImage>(L"EndingIMG");
  _backIMG->SetSprite("2D\\UI\\Credit_Back.png", {960, 540});

  _endingIMG = CreateUI<UIImage>(L"EndingIMG");
  _endingIMG->SetSprite("2D\\UI\\Credit_Final.png", _IMGpos);

  _vignetteIMG = CreateUI<UIImage>(L"EndingIMG");
  _vignetteIMG->SetSprite("2D\\UI\\UI_vignette_01.png", {960, 540});

  // Cursor
  _cursor = CreateUI<UICursor>(L"Cursor");
  _cursor->SetCursorType(CursorType::DEFAULT);
}

CreditUI::~CreditUI() {}

void CreditUI::BeginLevel()
{
  UIPanel::BeginLevel();
}

void CreditUI::Update(float dt)
{
  UIPanel::Update(dt);

  _IMGpos.y = _IMGpos.y - dt * _speed;
  _endingIMG->SetCenterPos(_IMGpos);

  if (_IMGpos.y <= - 1300.0f)
  {
    _world->PrepareChangeLevel("Main Menu");
    _world->CommitLevelChange();
  }
}
