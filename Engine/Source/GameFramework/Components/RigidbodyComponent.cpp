#include "GameFramework/GameObject/GameObject.h"
#include "GameFramework/Components/RigidbodyComponent.h"

#include "GameFramework/World/World.h"

#include "PhyjixWorld.h"

#include <DirectXMath.h>
#ifdef _DEBUG
float RigidbodyComponent::scalingFactor = 2.0f;
#endif
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
                 DirectX::XMMatrixTranslationFromVector(offsetTranslation);

  isStatic = isStatic;
  isKinematic = isKinematic;
  RegisterRigidBodyToWorld();
}

void RigidbodyComponent::SetCollisionEvent(IRigidBody* other,
                                           eCollisionEventType eventType, Event event)
{
  _rigidbody->SetCollisionEvent(eventType, other, event);
}

void RigidbodyComponent::RemoveCollisionEvent(IRigidBody* other)
{
  _rigidbody->RemoveCollisionEvent(other);
}


void RigidbodyComponent::SetOffsetTransform(const DirectX::SimpleMath::Vector3& offsetTranslation,
                                            const Quaternion& offsetQuaternion,
                                            const Vector3& offsetSize)
{

  _rigidbody->SetWorldTransform(offsetTranslation, offsetQuaternion);
}



RigidbodyComponent::~RigidbodyComponent() {
  UnregisterRigidBodyFromWorld();

  _world->RemoveRigidBody(_rigidbody);

  delete _rigidbody;
  _rigidbody = nullptr;
  _world = nullptr;
}

void RigidbodyComponent::Translate(Vector3 Force)
{
  _rigidbody->SetLinVelocity(Force);
}

void RigidbodyComponent::Rotate(Vector3 Torque) 
{
  _rigidbody->SetAngVelocity(Torque);
}

void RigidbodyComponent::SetKinematicTransform(Vector3 pos, Quaternion rot)
{
  _rigidbody->KinematicMoveTo(pos, rot);
}

void RigidbodyComponent::UpdateKinematicTransform()
{
  SetKinematicTransform(GetTransformComponent()->GetGlobalTranslation(),
                        GetTransformComponent()->GetGlobalQuaternion());
}

void RigidbodyComponent::SetWorldTransform(Vector3 pos, Quaternion rot)
{
  _rigidbody->SetWorldTransform(pos, rot);
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
  if (isSimulationEnabled)
    return;

  _rigidbody->EnableSimulation();
  isKinematic = true;
  isSimulationEnabled = true;
}

void RigidbodyComponent::DisableSimulation()
{
  if (!isSimulationEnabled)
    return;

  _rigidbody->DisableSimulation();
  isKinematic = false;
  isSimulationEnabled = false;
}

void RigidbodyComponent::UpdateFromTransform()
{
    _rigidbody->SetWorldTransform(GetTransformComponent()->GetGlobalTranslation(),
        GetTransformComponent()->GetGlobalQuaternion());
}

void RigidbodyComponent::UpdateToTransform()
{

  XMVECTOR _S = GetTransformComponent()->GetScaling();
  XMVECTOR _R = _rigidbody->GetWorldRotation();
  XMVECTOR _T = _rigidbody->GetWorldPosition();

  XMMATRIX transform = XMMatrixScalingFromVector(_S)*
                       XMMatrixRotationQuaternion(_R) *
                       XMMatrixTranslationFromVector(_T);
  //GetTransformComponent()->globalTransform = transform;
}

void RigidbodyComponent::EnableDebugDraw()
{
#ifdef _DEBUG
  _bDebugDrawFlag = true;
#endif
}

void RigidbodyComponent::DisableDebugDraw()
{
#ifdef _DEBUG
  _bDebugDrawFlag = false;
#endif
}

void RigidbodyComponent::UpdateDebugDrawMatrix()
{
#ifdef _DEBUG

  debugDrawMatrix = XMMatrixScalingFromVector(offsetScale*scalingFactor) *
                    XMMatrixRotationQuaternion(offsetRotation) *
                    XMMatrixTranslationFromVector(offsetTranslation) *
                    XMMatrixRotationQuaternion(
                        GetTransformComponent()->GetGlobalQuaternion()) *
                    XMMatrixTranslationFromVector(
                        GetTransformComponent()->GetGlobalTranslation());
#endif
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

void RigidbodyComponent::Hover()
{
  GetOwner()->OnHover();
}

void RigidbodyComponent::LeftClick()
{
  GetOwner()->OnLeftClick();
}

void RigidbodyComponent::RightClick()
{
  GetOwner()->OnRightClick();
}




TransformComponent* RigidbodyComponent::GetTransformComponent()
{
  return GetOwner()->transform;
}
