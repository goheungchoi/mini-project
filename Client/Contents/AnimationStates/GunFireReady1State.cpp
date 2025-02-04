#include "GunFireReady1State.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void GunFireReady1State::Toggle(AnimatorComponent* animator) {
  animator->SetVariable<bool>("arm", true);

  if (animator->GetVariable<bool>("dead"))
  {
    animator->SetState(_stateDependency["dead"]);
    return;
  }

	if (animator->GetVariable<bool>("triggered"))
  {
		animator->SetState(_stateDependency["next"]);
	}
}