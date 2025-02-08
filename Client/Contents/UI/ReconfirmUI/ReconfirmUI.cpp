#include "ReconfirmUI.h"
#include "GameFramework/UI/UIImage/UIImage.h"

ReconfirmUI::ReconfirmUI(World* world) : UIPanel(world)
{
  _testImg = CreateUI<UIImage>(L"TestIMG");
  _testImg->SetSprite("2D\\UI\\UI_Stage_L.png", {960,540});
  _testImg->SetScale({2, 2});
}

ReconfirmUI::~ReconfirmUI() {}

