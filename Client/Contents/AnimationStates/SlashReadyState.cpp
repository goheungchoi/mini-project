#include "SlashReadyState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void SlashReadyState::Toggle(AnimatorComponent* animator) {
  animator->SetVariable<bool>("arm", true);

  if (animator->GetVariable<bool>("dead"))
  {
    animator->SetState(_stateDependency["dead"]);
    return;
  }

  animator->SetState(_stateDependency["next"]);
}
