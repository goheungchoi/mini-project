#pragma once

#include "GameFramework/Components/Animation/AnimationState.h"

class BrawlerActionState : public AnimationState
{

public:
  BrawlerActionState(Animation* animation) : AnimationState(animation) {}

  void Toggle(AnimatorComponent*) override;
};
