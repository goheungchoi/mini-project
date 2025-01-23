#include "GameFramework/Components/SkeletalMeshComponent.h"

#include "ResourceManager/ResourceManager.h"

#include "GameFramework/GameObject/GameObject.h"

#include "GameFramework/World/World.h"

void SkeletalMeshComponent::RegisterMeshToWorld() {
  GetOwner()->GetWorld()->RegisterMeshComponent(this);
}

void SkeletalMeshComponent::UpdateBoneTransforms()
{
  if (!rootBone)
    return;

  // Update the transform information of the child game objects;
  std::stack<TransformComponent*> transformStack;

  TransformComponent* rootTransform = rootBone;
  transformStack.push(rootTransform);

  boneTransforms[0] = XMMatrixTranspose(rootTransform->globalTransform);

  uint32_t index = 0;
  while (!transformStack.empty())
  {
    TransformComponent* currTransform = transformStack.top();
    transformStack.pop();

    for (auto* child : currTransform->children)
    {
      if (auto it = boneNameSet.find(child->GetOwner()->name);
          it != boneNameSet.end())
        boneTransforms[++index] = XMMatrixTranspose(child->globalTransform);
    }

    for (auto rit = currTransform->children.rbegin();
         rit != currTransform->children.rend(); ++rit)
    {
      transformStack.push(*rit);
    }
  }
}
