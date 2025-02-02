#include "BrawlerActionState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void BrawlerActionState::Toggle(AnimatorComponent* animator) {
  if (animator->GetVariable<bool>("dead"))
  {
    animator->SetState(_stateDependency["dead"]);
    return;
  }
}