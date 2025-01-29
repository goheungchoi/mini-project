#include "SlashActionState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void SlashActionState::Toggle(AnimatorComponent* animator) {
  animator->SetState(_stateDependency["idle"]);
}

