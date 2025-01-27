#include "GameFramework/GameObject/GameObject.h"
#include "GameFramework/Components/RigidbodyComponent.h"

#include "../../../../PhyjixEngine/PhyjixWorld.h"

void RigidbodyComponent::Initialize(
    const DirectX::SimpleMath::Vector3& position,
    const DirectX::SimpleMath::Vector3& size, ColliderShape cShape,
    BOOL isStatic, BOOL isKinematic, IPhyjixWorld* world)
{
  _world = world;
  _rigidbody = _world->AddRigidBody(position, size, cShape, isStatic, isKinematic);

}

void RigidbodyComponent::SetCollisionEvent(IRigidBody* other,
                                           eCollisionEventType eventType, Event event)
{
  _rigidbody->SetCollisionEvent(eventType, other, event);
}

void RigidbodyComponent::Translate(Vector3 Force)
{
  _rigidbody->SetLinVelocity(Force);
}

void RigidbodyComponent::Rotate(Vector3 Torque) 
{
  _rigidbody->SetAngVelocity(Torque);
}

void RigidbodyComponent::ClearForce()
{
  Translate({0, 0, 0});
}

void RigidbodyComponent::ClearTorque()
{
  Rotate({0, 0, 0});
}

void RigidbodyComponent::EnableCollision()
{
  _rigidbody->EnableCollision();
}

void RigidbodyComponent::DisableCollision()
{
  _rigidbody->DisableCollision();
}

void RigidbodyComponent::EnableGravity()
{
  _rigidbody->EnableGravity();
}

void RigidbodyComponent::DisableGravity()
{
  _rigidbody->DisableGravity();
}
#ifdef _DEBUG

void RigidbodyComponent::UpdateFromTransform()
{
  _rigidbody->SetWorldTransform(GetTransformComponent()->GetTranslation(),
                                GetTransformComponent()->GetQuaternion());
  _prevTransform = _rigidbody->GetWorldTransform();


}

void RigidbodyComponent::UpdateToTransform()
{
  XMVECTOR _S = GetTransformComponent()->GetScaling();
  XMVECTOR _R = _rigidbody->GetWorldRotation();
  XMVECTOR _T = _rigidbody->GetWorldPosition();

  XMMATRIX transform = XMMatrixScalingFromVector(_S) *
                       XMMatrixRotationQuaternion(_R) *
                       XMMatrixTranslationFromVector(_T);
  GetTransformComponent()->SetLocalTransform(transform);
}

void RigidbodyComponent::EnableDebugDraw()
{
  _bDebugDrawFlag = true;
}

void RigidbodyComponent::DisableDebugDraw()
{
  _bDebugDrawFlag = false;
}
#endif
TransformComponent* RigidbodyComponent::GetTransformComponent()
{
  return GetOwner()->transform;
}