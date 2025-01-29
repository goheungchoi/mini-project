#pragma once

#include "GameFramework/Components/Animation/AnimationState.h"

class SlashActionState : public AnimationState
{

public:
  SlashActionState(Animation* animation) : AnimationState(animation) {}

  void Toggle(AnimatorComponent*) override;
};
