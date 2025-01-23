#pragma once

#include "ComponentBase.h"

#include "GameFramework/Components/TransformComponent.h"

#include "Core/Handle/ResourceHandle.h"

class SkeletalMeshComponent : public ComponentBase
{
public:
  bool isVisible{true};
  bool bCastShadow{true};

  std::vector<XMMATRIX> boneTransforms;
  class TransformComponent* rootBone{nullptr};
  MeshHandle handle;

  SkeletalMeshComponent(class GameObject* owner) : ComponentBase(owner) {}

  void SetHandle(MeshHandle handle) { 
    const MeshData& data = AccessMeshData(handle);
    boneTransforms.resize(data.bones.size());
    this->handle = handle; 
  }
  void SetRootTransform(TransformComponent* root) { rootBone = root; }

  MeshHandle GetHandle() { return handle; }
  void RegisterMeshToWorld();
  void SetVisible(bool visible) { isVisible = visible; }
  void SetCastShadow(bool shadow) { bCastShadow = shadow; }

  void UpdateBoneTransforms()
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
        boneTransforms[++index] = XMMatrixTranspose(child->globalTransform);
      }

      for (auto rit = currTransform->children.rbegin();
           rit != currTransform->children.rend(); ++rit)
      {
        transformStack.push(*rit);
      }
    }
  }
};
