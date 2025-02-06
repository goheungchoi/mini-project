#include "SlashActionState.h"

#include "GameFramework/GameObject/GameObject.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"

#include "Contents/GameObjects/Map/Characters/Slasher/Slasher.h"

void SlashActionState::Enter(AnimatorComponent* animator)
{
  Super::Enter(animator);

  animator->SetVariable<bool>("fire", true);
}

void SlashActionState::Toggle(AnimatorComponent* animator)
{
  Slasher* slasher = (Slasher*) animator->GetOwner();
  slasher->TakeOverTargetCell();

  animator->SetVariable<bool>("done", true);
  animator->SetState(_stateDependency["idle"]);
}
