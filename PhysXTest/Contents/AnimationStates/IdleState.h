#pragma once

#include "GameFramework/Components/Animation/AnimationState.h"

class IdleState : public AnimationState
{

public:
  IdleState(Animation* animation) : AnimationState(animation) {}

  void Toggle(AnimatorComponent*) override;
};
