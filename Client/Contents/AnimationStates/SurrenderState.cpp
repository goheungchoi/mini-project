#include "SurrenderState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void SurrenderState::Toggle(AnimatorComponent* animator)
{
  if (animator->GetVariable<bool>("dead"))
  {
    animator->SetState(_stateDependency["dead"]);
    return;
  }
}