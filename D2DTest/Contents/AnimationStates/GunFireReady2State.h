#pragma once

#include "GameFramework/Components/Animation/AnimationState.h"

class GunFireReady2State : public AnimationState
{

public:
  GunFireReady2State(Animation* animation) : AnimationState(animation) {}

  void Toggle(AnimatorComponent*) override;
};
