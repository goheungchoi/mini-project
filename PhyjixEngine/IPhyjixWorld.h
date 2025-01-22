#pragma once
#include "physx/PxPhysicsAPI.h"
using namespace physx;
class IRigidBody;
class ICharacterController;

enum class ColliderShape
{
  eCubeCollider,
  eSphereCollider,
  eCapsuleCollider
};

class IPhyjixWorld
{
public:
  virtual ~IPhyjixWorld() = default;

      // object managing
  virtual IRigidBody* AddRigidBody(const DirectX::SimpleMath::Vector3& position,
                                   const DirectX::SimpleMath::Vector3& size,
                                   ColliderShape shape, bool isStatic = false,
                                   bool isKinematic = true) = 0;
  virtual void RemoveRigidBody(IRigidBody* body) = 0;

  virtual ICharacterController* CreateCharacterController(
      const DirectX::SimpleMath::Vector3& position, float radius,
      float height) = 0;
  virtual void RemoveCharacterController(ICharacterController* controller) = 0;
  virtual void UpdateCharacterControllers(float deltaTime) = 0;

  virtual void CreateDefaultGround() = 0;

  // physics update
  virtual void Update(float deltaTime) = 0;
};
