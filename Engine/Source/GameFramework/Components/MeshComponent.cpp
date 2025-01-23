#include "GameFramework/Components/MeshComponent.h"

#include "GameFramework/World/World.h"

void MeshComponent::RegisterMeshToWorld() {
  GetOwner()->GetWorld()->RegisterMeshComponent(this);
}
