#include "GameFramework/Components/Animation/AnimatorComponent.h"

#include "GameFramework/Components/Animation/Animation.h"

#include "GameFramework/Components/TransformComponent.h"

#include "GameFramework/GameObject/GameObject.h"


 void AnimatorComponent::UpdateAnimation(float dt) {
	// TODO:
 }

void AnimatorComponent::Toggle()
{
  _currState->Toggle(this);
}
void AnimatorComponent::SetState(AnimationState* state)
{
  _currState->Exit(this);
  _currState = state;
  _currState->Enter(this);
}