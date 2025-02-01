#include "SlashReadyState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void SlashReadyState::Toggle(AnimatorComponent* animator) {
  animator->SetState(_stateDependency["next"]);
}
