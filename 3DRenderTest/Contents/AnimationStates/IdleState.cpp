#include "IdleState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void IdleState::Toggle(AnimatorComponent* animator) {
  if (animator->GetVariable<bool>("dead"))
  {
    animator->SetState(_stateDependency["dead"]);
    return;
  }

	if (animator->GetVariable<bool>("triggered"))
  {
    if (_animation->IsLoop())
    {
      _animation->SetLoop(false);
      _animation->Trigger();
    }
    else
    {
      _animation->SetLoop(true);
      animator->SetState(_stateDependency["next"]);
    }
	}
}
