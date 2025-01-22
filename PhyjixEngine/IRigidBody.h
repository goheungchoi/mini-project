#pragma once
#include "IPhyjixWorld.h"
#include "physx/PxPhysicsAPI.h"
using namespace physx;
enum class eCollisionEventType
{
  eCollisionEnter,
  eCollisionExit,
  eTrigger,
  eWake,
  eSleep,
  eCollisionTypeEnd,
};

class IRigidBody
{

public:
  virtual ~IRigidBody() = default;

  virtual void SetCollisionEvent(eCollisionEventType collisiontype,
                                 IRigidBody* other,
                                 std::function<void(void)> event) = 0;

  virtual void SetLinVelocity(DirectX::SimpleMath::Vector3 vel) = 0;
  virtual void SetMaxLinVelocity(float vel) = 0;
  virtual void SetAngVelocity(DirectX::SimpleMath::Vector3 vel) = 0;
  virtual void SetMaxAngVelocity(float vel) = 0;

  virtual DirectX::SimpleMath::Vector3 GetLinVelocity() = 0;
  virtual float GetMaxLinVelocity() = 0;
  virtual DirectX::SimpleMath::Vector3 GetAngVelocity() = 0;
  virtual float GetMaxAngVelocity() = 0;

  virtual void EnableCollision() = 0;
  virtual void DisableCollision() = 0;

  virtual void EnableGravity() = 0;
  virtual void DisableGravity() = 0;

  virtual void WakeUp() = 0;
  virtual void Sleep() = 0;

  virtual ColliderShape GetColliderShapeType() = 0;

};
