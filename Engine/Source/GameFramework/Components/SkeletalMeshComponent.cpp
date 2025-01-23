#include "GameFramework/Components/SkeletalMeshComponent.h"

#include "ResourceManager/ResourceManager.h"

#include "GameFramework/World/World.h"

void SkeletalMeshComponent::RegisterMeshToWorld()
{
  GetOwner()->GetWorld()->RegisterMeshComponent(this);
}

