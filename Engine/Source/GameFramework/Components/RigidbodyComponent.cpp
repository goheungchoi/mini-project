#include "GameFramework/GameObject/GameObject.h"
#include "GameFramework/Components/RigidbodyComponent.h"

#include "GameFramework/World/World.h"

#include "PhyjixWorld.h"
float RigidbodyComponent::scalingFactor = 2.0f;
void RigidbodyComponent::Initialize(
    const DirectX::SimpleMath::Vector3& _offsetTranslation,
    const DirectX::SimpleMath::Quaternion& offsetQuaternion,
    const DirectX::SimpleMath::Vector3& offsetSize, ColliderShape cShape,
    BOOL isStatic, BOOL isKinematic, IPhyjixWorld* world)
{
  _world = world;
  _rigidbody = _world->AddRigidBody(GetTransformComponent()->GetGlobalTranslation(),GetTransformComponent()->GetGlobalQuaternion(),
      _offsetTranslation, offsetQuaternion,
                                    offsetSize,
                                    cShape, isStatic,
                                    isKinematic);
  offsetScale = offsetSize;
  offsetRotation = offsetQuaternion;
  offsetTranslation = _offsetTranslation;
  offsetMatrix = XMMatrixScalingFromVector(offsetScale) *
                 XMMatrixRotationQuaternion(offsetRotation) *
                 XMMatrixTranslationFromVector(offsetTranslation);

  RegisterRigidBodyToWorld();
}

void RigidbodyComponent::SetCollisionEvent(IRigidBody* other,
                                           eCollisionEventType eventType, Event event)
{
  _rigidbody->SetCollisionEvent(eventType, other, event);
}

void RigidbodyComponent::SetOffsetTransform(const Vector3& offsetTranslation,
                                            const Quaternion& offsetQuaternion,
                                            const Vector3& offsetSize)
{

  _rigidbody->SetWorldTransform(offsetTranslation, offsetQuaternion);
}



RigidbodyComponent::~RigidbodyComponent() {
  _world->RemoveRigidBody(_rigidbody);
  UnregisterRigidBodyFromWorld();
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

void RigidbodyComponent::EnableSimulation()
{
  _rigidbody->EnableSimulation();
}

void RigidbodyComponent::DisableSimulation()
{
  _rigidbody->DisableSimulation();
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

  XMMATRIX transform = XMMatrixScalingFromVector(_S)*
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

void RigidbodyComponent::UpdateDebugDrawMatrix()
{
  debugDrawMatrix = XMMatrixScalingFromVector(offsetScale*scalingFactor) *
                    XMMatrixRotationQuaternion(offsetRotation) *
                    XMMatrixTranslationFromVector(offsetTranslation) *
                    XMMatrixRotationQuaternion(
                        GetTransformComponent()->GetGlobalQuaternion()) *
                    XMMatrixTranslationFromVector(
                        GetTransformComponent()->GetGlobalTranslation());
}

void RigidbodyComponent::RegisterRigidBodyToWorld() {
  GetWorld()->RegisterRigidBodyComponent(this);
}

void RigidbodyComponent::UnregisterRigidBodyFromWorld() {
  GetWorld()->UnregisterRigidBodyComponent(this);
}

void RigidbodyComponent::BeginOverlap(RigidbodyComponent* other)
{
  GetOwner()->OnBeginOverlap(other->GetOwner());
}

void RigidbodyComponent::EndOverlap(RigidbodyComponent* other)
{
  GetOwner()->OnEndOverlap(other->GetOwner());
}

#endif
TransformComponent* RigidbodyComponent::GetTransformComponent()
{
  return GetOwner()->transform;
}
