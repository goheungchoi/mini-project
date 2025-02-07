#include "TransitionUI.h"

#include "../../../../Engine/Source/Renderer/D2DRenderer/Sprite/Sprite.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "Shared/Config/Config.h"

TransitionUI::TransitionUI(class World* world) : UIPanel(world)
{
  _blackImage = CreateUI<UIImage>(L"FadeImage");
  _blackImage->SetSprite("2D\\UI\\FadeBlack.png");
  _blackImage->SetCenterPos({960, 540});
  _blackImage->SetScale({120, 120});


}

void TransitionUI::SetTransitionLength(float time)
{
  
}
void TransitionUI::FadeOut()
{
  
}
void TransitionUI::FadeIn()
{
  
}
void TransitionUI::Update(float dt)
{
  UIPanel::Update(dt);
}
void TransitionUI::Render()
{
  UIPanel::Render();
}