#include "GameFramework/Components/MeshComponent.h"

#include "GameFramework/World/World.h"

void MeshComponent::RegisterMeshToWorld() {
  GetOwner()->GetWorld()->RegisterMeshComponent(this);
}

void MeshComponent::SetCastShadow(bool shadow)
{
  for (MeshHandle handle : subMeshHandles)
  {
    if (shadow)
    {
      GetWorld()->_renderer->AddShadow(handle);
    }
    else
    {
      GetWorld()->_renderer->DeleteShadow(handle);
    }
  }
  
  bCastShadow = shadow;
}
