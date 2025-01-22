#include "GameFramework/Components/Animation/AnimatorComponent.h"

#include "GameFramework/Components/Animation/Animation.h"

#include "GameFramework/Components/TransformComponent.h"

#include "GameFramework/GameObject/GameObject.h"


 void AnimatorComponent::UpdateAnimation(float dt)
{
  if (_currState)
  {
    if (skeleton)
    {
      _currState->_animation->UpdateBoneTransforms(dt, skeleton,
                                                   finalBoneTransforms);

      // Update the transform information of the child game objects;
      std::stack<TransformComponent*> transformStack;

      TransformComponent* rootTransform = GetOwner()->transform;
      transformStack.push(rootTransform);

      uint32_t boneId = 0;
      uint32_t index = 0;
      while (!transformStack.empty())
      {
        TransformComponent* currTransform = transformStack.top();
        transformStack.pop();

        /*std::string nodeName = currTransform->GetOwner()->name;
        std::string skeletalNodeName = skeleton->nodes[index].name;
        if (nodeName != skeletalNodeName)
        {
          throw std::runtime_error("Node name mismatch!");
        }*/

        for (auto* child : currTransform->children)
        {
          std::string nodeName = child->GetOwner()->name;
          std::string skeletalNodeName = skeleton->nodes[index + 1].name;
          if (skeleton->nodes[++index].boneId >= 0)
            child->globalTransform = finalBoneTransforms[boneId++];
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
      _currState->_animation->Update(dt);

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
        currTransform->localTransform = channel->GetLocalTransform();

        for (auto* child : currTransform->children)
        {
          transformStack.push(child);
        }
      }
    }
  }
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