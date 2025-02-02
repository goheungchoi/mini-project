#include "GunFireActionState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void GunFireActionState::Enter(AnimatorComponent* animator)
{
  Super::Enter(animator);

  animator->SetVariable<bool>("fire", true);
}

void GunFireActionState::Toggle(AnimatorComponent* animator) {
  if (animator->GetVariable<bool>("dead"))
  {
    animator->SetState(_stateDependency["dead"]);
    return;
  }
  // animator->SetState(_stateDependency["idle"]);
}


