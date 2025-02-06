#include "BrawlerActionState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void BrawlerActionState::Toggle(AnimatorComponent* animator) {
  animator->SetVariable<bool>("done", true);

  if (animator->GetVariable<bool>("dead"))
  {
    animator->SetState(_stateDependency["dead"]);
    return;
  }

  animator->SetState(_stateDependency["idle"]);
}
