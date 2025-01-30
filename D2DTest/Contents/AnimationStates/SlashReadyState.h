#pragma once

#include "GameFramework/Components/Animation/AnimationState.h"

class SlashReadyState : public AnimationState
{

public:
  SlashReadyState(Animation* animation) : AnimationState(animation) {}

  void Toggle(AnimatorComponent*) override;
};
