#pragma once
#include "GameFramework\UI\UIPanel\UIPanel.h"

class TransitionUI : public UIPanel
{

public:
  class UIImage* _blackImage{nullptr};
  float _transitionElaspedTimer = 0.f;

  TransitionUI(class World* world);
  virtual ~TransitionUI() = default;

  void SetTransitionLength(float time);
  void FadeOut();
  void FadeIn();
  void Update(float dt) override;
  void Render() override;
private:
  ;
};
