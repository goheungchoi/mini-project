#pragma once
#include "ICharacterController.h"
class CharacterController : public ICharacterController
{
public:
	CharacterController();
	~CharacterController();

	void Initialize(physx::PxControllerManager* manager, const physx::PxCapsuleControllerDesc& desc);
	void Release();


	physx::PxControllerCollisionFlags Move(const physx::PxVec3& displacement, float minDist, float elapsedTime);
	physx::PxExtendedVec3 GetPosition() const;
	void SetMoveDirection(const physx::PxExtendedVec3& position);


	void SetPosition(const physx::PxExtendedVec3& position);
	float GetHeight() const;
	void SetHeight(float height);
	float GetRadius() const;
	void SetRadius(float radius);
	bool IsGrounded() const;

	void Update(float deltaTime);


private:
	physx::PxExtendedVec3 m_MoveDirection;
	physx::PxController* m_controller;
	physx::PxControllerManager* m_manager;
	physx::PxControllerCollisionFlags m_lastCollisionFlags;


};

