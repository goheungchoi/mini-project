#pragma once

#include "ComponentBase.h"

#include "Core/Handle/ResourceHandle.h"

#include "Core/Types/RenderType.h"

class MeshComponent : public ComponentBase
{
public:
  bool isVisible{true};
  bool bCastShadow{true};
  RenderTypeFlags renderTypeFlags{};
  Color outlineColor{};
  std::vector<MeshHandle> subMeshHandles;

	MeshComponent(class GameObject* owner) : ComponentBase(owner) {}

	void AddSubMesh(MeshHandle handle) { subMeshHandles.push_back(handle); }
  void RemoveSubMesh(MeshHandle handle) { std::erase(subMeshHandles, handle); }
  const auto& GetSubMeshes() { return subMeshHandles; }

  void RegisterMeshToWorld();
  void SetVisible(bool visible) { isVisible = visible; }
  void SetCastShadow(bool shadow);

  void ShowOutline() { renderTypeFlags = kOutline; }
  void HideOutline() { renderTypeFlags = RenderTypeNone; }
};
