#pragma once
#include "IPhyjixWorld.h"
enum class eCollisionEventType
{
  eCollisionEnter,
  eCollisionExit,
  eTrigger,
  eWake,
  eSleep,
  eHover,
  eLClick,
  eRClick,
  eOverlapBegin,
  eOverlapEnd,
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

  virtual void KinematicMoveTo(DirectX::SimpleMath::Vector3 pos,
                               DirectX::SimpleMath::Vector4 rot) = 0;

  virtual void EnableCollision() = 0;
  virtual void DisableCollision() = 0;

  virtual void EnableGravity() = 0;
  virtual void DisableGravity() = 0;

  virtual void EnableSimulation() = 0;
  virtual void DisableSimulation() = 0;

  virtual void WakeUp() = 0;
  virtual void Sleep() = 0;

  virtual bool isKinematic() = 0;

  virtual physx::PxTransform GetWorldTransform() = 0;
  virtual DirectX::SimpleMath::Vector3 GetWorldPosition() = 0;
  virtual DirectX::SimpleMath::Vector4 GetWorldRotation() = 0;
  virtual void SetWorldTransform(
      DirectX::SimpleMath::Vector3 pos,DirectX::SimpleMath::Vector4 rot) = 0;


  virtual ColliderShape GetColliderShapeType() = 0;

};
