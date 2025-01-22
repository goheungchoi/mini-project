#pragma once
#include "physx/PxPhysicsAPI.h"
using namespace physx;
class ICharacterController
{
public:
	virtual ~ICharacterController() = default;

    virtual void Initialize(physx::PxControllerManager* manager, const physx::PxCapsuleControllerDesc& desc) = 0;
    virtual void Release() = 0;
    virtual physx::PxControllerCollisionFlags Move(const physx::PxVec3& displacement, float minDist, float elapsedTime) = 0;
    virtual physx::PxExtendedVec3 GetPosition() const = 0;
    virtual void SetPosition(const physx::PxExtendedVec3& position) = 0;
    virtual void SetMoveDirection(const physx::PxExtendedVec3& position) = 0;
    virtual float GetHeight() const = 0;
    virtual void SetHeight(float height) = 0;
    virtual float GetRadius() const = 0;
    virtual void SetRadius(float radius) = 0;
    virtual bool IsGrounded() const = 0;
   virtual void Update(float deltaTime) = 0; 

};

