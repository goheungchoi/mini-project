#include "GunFireActionState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void GunFireActionState::Toggle(AnimatorComponent* animator) {
  if (animator->GetVariable<bool>("dead"))
  {
    animator->SetState(_stateDependency["dead"]);
    return;
  }
  // animator->SetState(_stateDependency["idle"]);
}