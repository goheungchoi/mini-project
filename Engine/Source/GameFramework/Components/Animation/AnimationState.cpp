#include "GameFramework/Components/Animation/AnimationState.h"

#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimatorComponent.h"

AnimationState::AnimationState(Animation* animation)
    : _animation{animation}
{
}

void AnimationState::Exit(AnimatorComponent* controller)
{
  // e.g. in the 'PlayerIdle' animation state
  //
  // _animation = _animationSceneRegistry["idle_exit"];
  _animation->Quit();
}

void AnimationState::Toggle(AnimatorComponent* controller)
{
  // e.g. in the 'PlayerIdle' animation state
  //
  // if (controller->GetVariable("Jump") == true) {
  //   controller->SetState(PlayerJump);
  // } else if (controller->GetVariable("Duck") == true) {
  //   controller->SetState(PlayerDuck);
  // }
  //
}

void AnimationState::Enter(AnimatorComponent* controller)
{
  // e.g. in the 'PlayerIdle' animation state
  //
  // _animation = _animationSceneRegistry["idle_exit"];
  _animation->Trigger();
}

void AnimationState::AddAnimationStateDependency(const StateName& name,
                                                 AnimationState* state)
{
  if (auto it = _stateDependency.find(name); it != _stateDependency.end())
  {
    throw std::invalid_argument("The state \'" + name + "\' already exists!");
  }

  _stateDependency.insert(std::make_pair(name, state));
}
