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

  // GetOwner()->transform->globalTransform = rootTransform->globalTransform;
  auto it = boneNameMap.find(rootBone->GetOwner()->name);
  if (it != boneNameMap.end())
		boneTransforms[0] = XMMatrixTranspose(it->second.inverseBindMatrix *
                                        rootTransform->globalTransform);

  uint32_t index = 1;
  while (!transformStack.empty())
  {
    TransformComponent* currTransform = transformStack.top();
    transformStack.pop();

    for (auto* child : currTransform->children)
    {
#ifndef NDEBUG
      std::string n = child->GetOwner()->name;
#endif // !NDEBUG
      if (index >= boneNameMap.size())
      {
        continue;
      }

      if (auto it = boneNameMap.find(child->GetOwner()->name);
          it != boneNameMap.end())
        boneTransforms[index++] = XMMatrixTranspose(it->second.inverseBindMatrix * child->globalTransform);
    }

    for (auto rit = currTransform->children.rbegin();
         rit != currTransform->children.rend(); ++rit)
    {
      transformStack.push(*rit);
    }
  }
}
