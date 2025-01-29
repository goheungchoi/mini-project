#pragma once

#include "GameFramework/Components/Animation/AnimationState.h"

class DeadState : public AnimationState
{

public:
  DeadState(Animation* animation) : AnimationState(animation) {}

  void Toggle(AnimatorComponent*) override;
};
