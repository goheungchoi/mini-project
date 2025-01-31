#include "GunFireReady2State.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void GunFireReady2State::Toggle(AnimatorComponent* animator) {
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