#include "GameFramework/Components/Animation/AnimatorComponent.h"

#include "GameFramework/Components/Animation/Animation.h"

#include "GameFramework/Components/TransformComponent.h"

#include "GameFramework/GameObject/GameObject.h"

void AnimatorComponent::UpdateAnimation(float dt)
{
  this;

  if (!_currState)
    return;

  if (_currState->_animation->IsDone())
  {
    Toggle();
    return;
	}

	if (_currState->_animation->IsPaused())
  {
    dt = 0.f;
	}

  if (skeleton)
  {
    _currState->_animation->UpdateBoneTransforms(dt * playSpeed, skeleton,
                                                 finalBoneTransforms);

    // Update the transform information of the child game objects;
    std::stack<TransformComponent*> transformStack;

    TransformComponent* rootTransform = GetOwner()->transform;
    transformStack.push(rootTransform);

    uint32_t boneId = 0;
    uint32_t index = 1;
    while (!transformStack.empty())
    {
      TransformComponent* currTransform = transformStack.top();
      transformStack.pop();

      for (auto* child : currTransform->children)
      {
        if (index >= skeleton->nodes.size())
        {
          continue;
				}

        std::string nodeName = child->GetOwner()->name;
        std::string skeletalNodeName = skeleton->nodes[index].name;
        if (nodeName != skeletalNodeName)
        {
          continue;
        }

        if (skeleton->nodes[index++].boneId >= 0)
          child->SetLocalTransform(finalBoneTransforms[boneId++]);
      }

      for (auto rit = currTransform->children.rbegin();
           rit != currTransform->children.rend(); ++rit)
      {
        std::string nodeName = (*rit)->GetOwner()->name;
        transformStack.push(*rit);
      }
    }
  }
  else
  {
    // Update the current animation.
    _currState->_animation->Update(dt * playSpeed);

    // Update the transform information of the child game objects;
    std::stack<TransformComponent*> transformStack;

    TransformComponent* rootTransform = GetOwner()->transform;
    transformStack.push(rootTransform);

    while (!transformStack.empty())
    {
      TransformComponent* currTransform = transformStack.top();
      transformStack.pop();

      const auto* channel = _currState->_animation->FindChannel(
          currTransform->GetOwner()->GetName());
      currTransform->SetLocalTransform(channel->GetLocalTransform());

      for (auto* child : currTransform->children)
      {
        transformStack.push(child);
      }
    }
  }
}

void AnimatorComponent::PauseAnimation() {
  if (!_currState)
    return;

	_currState->GetAnimation()->Pause();
}

void AnimatorComponent::ResumeAnimation() {
  if (!_currState)
    return;

  _currState->GetAnimation()->Resume();
}

void AnimatorComponent::Toggle()
{
  _currState->Toggle(this);
}

void AnimatorComponent::SetState(AnimationState* state)
{
  if (_currState)
    _currState->Exit(this);
  _currState = state;
  _currState->Enter(this);
}