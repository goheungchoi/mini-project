#pragma once
#include "GameFramework\UI\UIPanel\UIPanel.h"

class TransitionUI : public UIPanel
{

public:
  class UIImage* _blackImage{nullptr};
  float _transitionElaspedTimer = 0.f;
  float _transitionThreshold = 1.f;
  float _transitionOpacity = 0.f;
  float _transitionStartOpacity = 0.f;
  float _transitionEndOpacity = 0.f;
  bool _transitionFlag = false;
  ;

  TransitionUI(class World* world);
  virtual ~TransitionUI() = default;

  void SetTransitionLength(float time);
  void FadeOut(float threshold =1.f);
  void FadeIn(float threshold = 1.f);
  void Transition(float dt);
  void Update(float dt) override;
  void Render() override;
private:

};
