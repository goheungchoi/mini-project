#pragma once

#include "GameFramework/Components/Animation/AnimationState.h"

class GunFireActionState : public AnimationState
{
	using Super = AnimationState;
public:
  GunFireActionState(Animation* animation) : AnimationState(animation) {}
  void Enter(AnimatorComponent*) override;
  void Toggle(AnimatorComponent*) override;
};
