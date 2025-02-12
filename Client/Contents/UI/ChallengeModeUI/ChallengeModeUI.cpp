#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "ChallengeModeUI.h"

ChallengeModeUI::ChallengeModeUI(World* world) : UIPanel(world)
{
  _backGroundImg = CreateUI<UIImage>(L"ChallengeModeBackGoundIMG");
  _backGroundImg->SetSprite("2D\\UI\\StageNum\\ChallengeBG.png", {960, 540});
}

ChallengeModeUI::~ChallengeModeUI()
{
  
}

