#include "SlashActionState.h"

#include "GameFramework/GameObject/GameObject.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

void SlashActionState::Enter(AnimatorComponent* animator) {
  Super::Enter(animator);

  animator->SetVariable<bool>("fire", true);
}

void SlashActionState::Toggle(AnimatorComponent* animator)
{
  animator->SetVariable<bool>("done", true);

  auto* animationRoot = animator->GetOwner()->FindChildGameObject("root_ref.x");
  XMVECTOR currTranslate = animationRoot->transform->GetTranslation();
  animator->SetVariable<XMVECTOR>("currTranslate", currTranslate);
  animator->SetState(_stateDependency["idle"]);
}

