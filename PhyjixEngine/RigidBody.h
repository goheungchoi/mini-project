#pragma once
#include "physx/PxPhysicsAPI.h"
#include "IRigidBody.h"
#include "ICollisionEvent.h"
#include "IPhyjixWorld.h"
using namespace physx;

class PhyjixWorld;
class RigidBody : public IRigidBody, public ICollisionEvent
{

  using CollisionEvent = std::function<void(RigidBody* self, RigidBody* other)>;

public:
  RigidBody(PxPhysics* physics, const DirectX::SimpleMath::Vector3& position,
            const DirectX::SimpleMath::Vector3& size, ColliderShape shape,
            BOOL isStatic, BOOL isKinematic, PhyjixWorld* world);
  ~RigidBody();
  //PxActor* GetActor() const;


// IRigidBody을(를) 통해 상속됨
  void SetCollisionEvent(eCollisionEventType collisiontype, IRigidBody* other, std::function<void(void)> event) override;
  void SetLinVelocity(DirectX::SimpleMath::Vector3 vel) override;
  void SetMaxLinVelocity(float vel) override;
  void SetAngVelocity(DirectX::SimpleMath::Vector3 vel) override;
  void SetMaxAngVelocity(float vel) override;
  DirectX::SimpleMath::Vector3 GetLinVelocity() override;
  float GetMaxLinVelocity() override;
  DirectX::SimpleMath::Vector3 GetAngVelocity() override;
  float GetMaxAngVelocity() override;
  void EnableCollision() override;
  void DisableCollision() override;
  void EnableGravity() override;
  void DisableGravity() override;
  void WakeUp() override;
  void Sleep() override;
  ColliderShape GetColliderShapeType() override;




  // ICollisionEvent을(를) 통해 상속됨
  void OnCollisionEnter(IRigidBody* other) override;
  void OnCollisionExit(IRigidBody* other) override;
  void OnTrigger(IRigidBody* other) override;
  void OnWake() override;
  void OnSleep() override;

  PxRigidActor* _actor = nullptr;

private:
  PhyjixWorld* _world = nullptr;
  PxShape* _defaultShape = nullptr;
  ColliderShape shape;

  //test
  PxShape* _nonElasticShape = nullptr;



  std::unordered_map<IRigidBody*, std::function<void(void)>> CollisionEnterEventMap;
  std::unordered_map<IRigidBody*, std::function<void(void)>> CollisionExitEventMap;
  std::unordered_map<IRigidBody*, std::function<void(void)>> TriggerEventMap;
  std::vector<std::function<void(void)>> WakeEventMap;
  std::vector<std::function<void(void)>> SleepEventMap;

  PxRigidDynamic* GetDynamicActor();


};
