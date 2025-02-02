#pragma once

#include "GameFramework/Components/Animation/AnimationState.h"

class GunFireReady1State : public AnimationState
{

public:
  GunFireReady1State(Animation* animation) : AnimationState(animation) {}

  void Toggle(AnimatorComponent*) override;
};
