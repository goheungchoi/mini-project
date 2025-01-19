#pragma once

#include "ComponentBase.h"

#include "Core/Handle/ResourceHandle.h"

class SkeletalMeshComponent : public ComponentBase
{
public:
  bool isVisible{true};
  bool bCastShadow{true};

  class TransformComponent* rootBone{nullptr};
  MeshHandle handle;

  SkeletalMeshComponent(class GameObject* owner) : ComponentBase(owner) {}

  MeshHandle GetHandle() { return handle; }
  void SetVisible(bool visible) { isVisible = visible; }
  void SetCastShadow(bool shadow) { bCastShadow = shadow; }
};
