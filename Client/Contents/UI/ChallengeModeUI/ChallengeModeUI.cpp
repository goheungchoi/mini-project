#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "ChallengeModeUI.h"

ChallengeModeUI::ChallengeModeUI(World* world) : UIPanel(world)
{
  _backGroundImg = CreateUI<UIImage>(L"vignette");
  _backGroundImg->SetSprite("2D\\UI\\Title\\Title_Background2.png", {960, 540});
  _backGroundImg->SetOpacity(1.0f);

  // Cursor
  _cursor = CreateUI<UICursor>(L"Cursor");
  _cursor->SetCursorType(CursorType::DEFAULT);
}

ChallengeModeUI::~ChallengeModeUI()
{
  
}

