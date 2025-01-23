#pragma once
#include "ComponentBase.h"
#include "TransformComponent.h"
#include "../../PhyjixEngine/RigidBody.h"

// component which controls the movement and simulate collision via physx;
// TransformComponent is effected and controlled by this component;
class RigidbodyComponent : public ComponentBase
{
  friend class TransformComponent;
  using Event = std::function<void(void)>;

public:

  //rigidbody initalize
  void Initialize(const DirectX::SimpleMath::Vector3& position,
                  const DirectX::SimpleMath::Vector3& size,
                  ColliderShape cShape, BOOL isStatic, BOOL isKinematic,
                  PhyjixWorld* world);

  //wrapper of the iRigidbody collision setting
  void SetCollisionEvent(IRigidBody* other, eCollisionEventType eventType, Event event);

  RigidbodyComponent(class GameObject* owner) : ComponentBase(owner){}

  void Translate(Vector3 force);
  void Rotate(Vector3 Torque);
  void ClearForce();
  void ClearTorque();
  void EnableCollision();
  void DisableCollision();
  void EnableGravity();
  void DisableGravity();
  IRigidBody* GetRigidBody() { return _rigidbody; };
#ifdef _DEBUG
  void EnableDebugDraw();
  void DisableDebugDraw();
  BOOL GetDebugDrawFlag() { return _bDebugDrawFlag; };
#endif


private:
  TransformComponent* GetTransformComponent();
  IRigidBody* _rigidbody = nullptr;
  PhyjixWorld* _world = nullptr;

#ifdef _DEBUG
  BOOL _bDebugDrawFlag = false;
#endif
};

