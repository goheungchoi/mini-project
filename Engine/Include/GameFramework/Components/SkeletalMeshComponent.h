#pragma once

#include "ComponentBase.h"

#include "GameFramework/Components/TransformComponent.h"

#include "Core/Handle/ResourceHandle.h"

#include "Core/Types/RenderType.h"

class SkeletalMeshComponent : public ComponentBase
{
public:
  bool isVisible{true};
  bool bCastShadow{true};
  RenderTypeFlags renderTypeFlags{};
  Color outlineColor{};

  std::unordered_map<std::string, Bone> boneNameMap;
  std::vector<XMMATRIX> boneTransforms;
  class TransformComponent* rootBone{nullptr};
  MeshHandle handle;

  SkeletalMeshComponent(class GameObject* owner) : ComponentBase(owner) {}

  void SetHandle(MeshHandle handle) { 
    const MeshData& data = AccessMeshData(handle);
    boneTransforms.resize(data.bones.size(), XMMatrixIdentity());
    boneNameMap.reserve(data.bones.size());
    for (auto& bone : data.bones)
    {
      boneNameMap.insert({bone.name, bone});
    }

    this->handle = handle; 
  }
  void SetRootTransform(TransformComponent* root) { rootBone = root; }

  MeshHandle GetHandle() { return handle; }
  void RegisterMeshToWorld();
  void SetVisible(bool visible) { isVisible = visible; }
  void SetCastShadow(bool shadow) { bCastShadow = shadow; }

  void UpdateBoneTransforms();
};
