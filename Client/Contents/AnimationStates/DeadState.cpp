#include "DeadState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void DeadState::Toggle(AnimatorComponent* animator) {
  animator->SetVariable<bool>("done", true);
}
