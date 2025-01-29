#pragma once

#include "GameFramework/Components/Animation/AnimationState.h"

class GunFireActionState : public AnimationState
{

public:
  GunFireActionState(Animation* animation) : AnimationState(animation) {}
  void Toggle(AnimatorComponent*) override;
};
