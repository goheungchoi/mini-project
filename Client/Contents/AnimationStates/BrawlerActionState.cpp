#include "BrawlerActionState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void BrawlerActionState::Toggle(AnimatorComponent* animator) {
  animator->SetVariable<bool>("done", true);
  animator->SetState(_stateDependency["idle"]);
}