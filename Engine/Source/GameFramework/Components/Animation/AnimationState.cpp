#include "GameFramework/Components/Animation/AnimationState.h"

#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimatorComponent.h"

#include "GameFramework/Components/TransformComponent.h"

#include "GameFramework/GameObject/GameObject.h"

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

void AnimationState::Update(AnimatorComponent* controller, float dt)
{
  if (controller->skeleton)
  {
    if (_animation)
    {
      _animation->UpdateBoneTransforms(dt, controller->skeleton,
                                       controller->finalBoneTransforms);

      // Update the transform information of the child game objects;
      std::queue<TransformComponent*> transformStack;

      TransformComponent* rootTransform = controller->GetOwner()->transform;
      transformStack.push(rootTransform);

      uint32_t index = 0;
      while (!transformStack.empty())
      {
        TransformComponent* currTransform = transformStack.front();
        transformStack.pop();

        currTransform->globalTransform = controller->finalBoneTransforms[index];

        for (auto* child : currTransform->children)
        {
          transformStack.push(child);
        }

        ++index;
      }
    }
  }
  else
  {
    // Update the current animation.
    _animation->Update(dt);

    // Update the transform information of the child game objects;
    std::stack<TransformComponent*> transformStack;

    TransformComponent* rootTransform = controller->GetOwner()->transform;
    transformStack.push(rootTransform);

    while (!transformStack.empty())
    {
      TransformComponent* currTransform = transformStack.top();
      transformStack.pop();

      const auto* channel =
          _animation->FindChannel(currTransform->GetOwner()->GetName());
      currTransform->localTransform = channel->GetLocalTransform();

      for (auto* child : currTransform->children)
      {
        transformStack.push(child);
      }
    }
  }
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
