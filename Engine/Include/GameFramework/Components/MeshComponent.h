#pragma once

#include "ComponentBase.h"

#include "Core/Handle/ResourceHandle.h"

class MeshComponent : public ComponentBase
{
public:
  bool isVisible{true};
  bool bCastShadow{true};
  MeshHandle handle;

	MeshComponent(class GameObject* owner) : ComponentBase(owner) {}

	MeshHandle GetHandle() { return handle; }
  void SetVisible(bool visible) { isVisible = visible; }
  void SetCastShadow(bool shadow) { bCastShadow = shadow; }
};
