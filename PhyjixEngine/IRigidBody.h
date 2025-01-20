#pragma once

class IRigidBody
{
public:
	virtual ~IRigidBody() = default;

	virtual void SetPosition(const DirectX::SimpleMath::Vector3& position) = 0;
	virtual DirectX::SimpleMath::Vector3 GetPosition() const = 0;

	virtual void SetRotation(const DirectX::SimpleMath::Vector4& rotation) = 0;
	virtual DirectX::SimpleMath::Vector4 GetRotation() const = 0;

	virtual void ApplyForce(const DirectX::SimpleMath::Vector3& force) = 0;

	virtual physx::PxRigidDynamic* GetActor() const = 0;
	virtual void ManGeunChu() = 0;
	virtual void ManGeunChuHaejae() = 0;
	//virtual void SetLinearVelocity()
};

