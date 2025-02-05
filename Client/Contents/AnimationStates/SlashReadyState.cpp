#include "SlashReadyState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void SlashReadyState::Toggle(AnimatorComponent* animator) {
  animator->SetVariable<bool>("arm", true);
  animator->SetState(_stateDependency["next"]);
}
