#include "ResultDialogUI.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"
#include "GameFramework/UI/UIText/UIText.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "Shared/Config/Config.h"
ResultDialogUI::ResultDialogUI(class World* world) : DialogUI(world)
{
  {
    _PhotoPanel = CreateUI<UIImage>(L"PhotoPanel");
    _PhotoPanel->SetSprite("Textures\\Picture\\FadeBlack.png");
    _PhotoPanel->SetScale({120,120});
    _PhotoPanel->SetOpacity(.5f);
    _PhotoPanel->SetCenterPos({960, 540});
  }
}


