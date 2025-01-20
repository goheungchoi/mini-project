#pragma once
#include "IRigidBody.h"

#include "IPhyjixWorld.h"
using namespace physx;

class PhyjixWorld;
class RigidBody :  public IRigidBody
{

	using CollisionEvent = std::function<void(RigidBody* self, RigidBody* other)>;

public:
	RigidBody(PxPhysics* physics, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& size , ColliderShape shape , BOOL isStatic, PhyjixWorld* world);
	~RigidBody();

	 
	void SetPosition(const DirectX::SimpleMath::Vector3& position) override;
	DirectX::SimpleMath::Vector3 GetPosition() const override;


	void SetRotation(const DirectX::SimpleMath::Vector4& rotation) override;
	DirectX::SimpleMath::Vector4 GetRotation() const override;

	//test
	void ManGeunChu();
	void ManGeunChuHaejae();
	bool mangeunchuflag = false;

	void ApplyForce(const DirectX::SimpleMath::Vector3& force) override;
	PxRigidDynamic* GetActor() const;

private:



	PxRigidActor* _actor = nullptr;
	PhyjixWorld* _world = nullptr;
	PxShape* _defaultShape = nullptr;
	PxShape* _nonElasticShape = nullptr;

};

