#pragma once

#include "GameFramework/Components/Animation/AnimationState.h"

class SlashActionState : public AnimationState
{
	using Super = AnimationState;
public:
  SlashActionState(Animation* animation) : AnimationState(animation) {}

	void Enter(AnimatorComponent*) override;
  void Toggle(AnimatorComponent*) override;
};
