#include "TransitionUI.h"

#include "../../../../Engine/Include/GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "Shared/Config/Config.h"

TransitionUI::TransitionUI(class World* world) : UIPanel(world)
{
  _blackImage = CreateUI<UIImage>(L"FadeImage");
  _blackImage->SetSprite("2D\\UI\\FadeBlack.png");
  _blackImage->SetSize({1920, 1080});
  _blackImage->SetCenterPos({960, 540});
  _blackImage->SetScale({120, 120});

}

void TransitionUI::SetTransitionLength(float time)
{
  _transitionThreshold = time;
}
void TransitionUI::FadeOut(float threshold)
{
  _transitionThreshold = threshold;
  _transitionStartOpacity = 0.f;
  _transitionEndOpacity = 1.f;
  _transitionFlag = true;
}
void TransitionUI::FadeIn(float threshold)
{
  _transitionThreshold = threshold;
  _transitionStartOpacity = 1.f;
  _transitionEndOpacity = 0.f;
  _transitionFlag = true;
}

void TransitionUI::Transition(float dt)
{
  if (_transitionFlag)
  {
    _transitionElaspedTimer += dt;
    if (_transitionElaspedTimer <= _transitionThreshold)
    {
      float transitionRatio = _transitionElaspedTimer / _transitionThreshold;
      _transitionOpacity =
          _transitionStartOpacity * (1 - transitionRatio)  +
                           _transitionEndOpacity * transitionRatio;
    }
    else
    {
      _transitionOpacity = _transitionEndOpacity;
      _transitionFlag = false;
      _transitionElaspedTimer = 0.f;
    }
  }


}

void TransitionUI::Update(float dt)
{
  UIPanel::Update(dt);
  //if (INPUT.IsKeyPress(Key::Space))
  //else if (INPUT.IsKeyPress(Key::N))
  //  FadeOut(5.f);


  Transition(dt);





}
void TransitionUI::Render()
{
  _blackImage->SetOpacity(_transitionOpacity);
  //UIPanel::Render();

  _blackImage->RenderTransition();
}