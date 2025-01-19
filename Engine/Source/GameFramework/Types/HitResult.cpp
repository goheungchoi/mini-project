#include "GameFramework/Types/HitResult.h"

#include "GameFramework/Components/Colliders/ColliderComponent.h"

GameObject* HitResult::GetHitGameObject()
{
  return hitComponent->GetOwner();
}
