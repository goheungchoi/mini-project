#pragma once

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

	virtual void SetCollisionEvent(eCollisionEventType collisiontype, IRigidBody* other,std::function<void(void)> event) = 0;
	
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& position) = 0;
	virtual DirectX::SimpleMath::Vector3 GetPosition() const = 0;

	virtual void SetRotation(const DirectX::SimpleMath::Vector4& rotation) = 0;
	virtual DirectX::SimpleMath::Vector4 GetRotation() const = 0;

	virtual void ApplyForce(const DirectX::SimpleMath::Vector3& force) = 0;





	virtual physx::PxRigidActor* GetActor() const = 0;
};

