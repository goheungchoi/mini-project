#pragma once

#include "GameFramework/Components/Animation/AnimationState.h"

class SurrenderState : public AnimationState
{

public:
  SurrenderState(Animation* animation) : AnimationState(animation) {}

  void Toggle(AnimatorComponent*) override;
};
