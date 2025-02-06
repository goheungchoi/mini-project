#include "SubMission.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"

SubMission::SubMission(World* world) : UIPanel(world)
{
  // SubMisson
  _subMissionImg = CreateUI<UIImage>(L"SubMissonImg");
  _subMissionImg->SetSprite("2D\\UI\\UI_Stage_S.png", {208, 240});
}

SubMission::~SubMission() {}
