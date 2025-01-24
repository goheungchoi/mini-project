#pragma once

#include "ComponentBase.h"

#include "Core/Handle/ResourceHandle.h"

class MeshComponent : public ComponentBase
{
public:
  bool isVisible{true};
  bool bCastShadow{true};
  std::vector<MeshHandle> subMeshHandles;

	MeshComponent(class GameObject* owner) : ComponentBase(owner) {}

	void AddSubMesh(MeshHandle handle) { subMeshHandles.push_back(handle); }
  void RemoveSubMesh(MeshHandle handle) { std::erase(subMeshHandles, handle); }
  const auto& GetSubMeshes() { return subMeshHandles; }

  void RegisterMeshToWorld();
  void SetVisible(bool visible) { isVisible = visible; }
  void SetCastShadow(bool shadow) { bCastShadow = shadow; }
};
