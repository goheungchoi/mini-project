#include "SlashActionState.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void SlashActionState::Enter(AnimatorComponent* animator) {
  Super::Enter(animator);

  animator->SetVariable<bool>("fire", true);
}

void SlashActionState::Toggle(AnimatorComponent* animator)
{
  animator->SetState(_stateDependency["idle"]);
}

