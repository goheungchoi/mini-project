#pragma once
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

// IRigidBody을(를) 통해 상속됨
  void SetCollisionEvent(eCollisionEventType collisiontype, IRigidBody* other,
                         std::function<void(void)> event) override;

  void SetPosition(const DirectX::SimpleMath::Vector3& position) override;
  DirectX::SimpleMath::Vector3 GetPosition() const override;

  void SetRotation(const DirectX::SimpleMath::Vector4& rotation) override;
  DirectX::SimpleMath::Vector4 GetRotation() const override;

  void ApplyForce(const DirectX::SimpleMath::Vector3& force) override;

  PxRigidActor* GetActor() const;

  // ICollisionEvent을(를) 통해 상속됨
  void OnCollisionEnter(IRigidBody* other) override;
  void OnCollisionExit(IRigidBody* other) override;
  void OnTrigger(IRigidBody* other) override;
  void OnWake() override;
  void OnSleep() override;

private:
  PxRigidActor* _actor = nullptr;
  PhyjixWorld* _world = nullptr;
  PxShape* _defaultShape = nullptr;
  PxShape* _nonElasticShape = nullptr;

  std::unordered_map<IRigidBody*, std::function<void(void)>> CollisionEnterEventMap;
  std::unordered_map<IRigidBody*, std::function<void(void)>> CollisionExitEventMap;
  std::unordered_map<IRigidBody*, std::function<void(void)>> TriggerEventMap;
  std::vector<std::function<void(void)>> WakeEventMap;
  std::vector<std::function<void(void)>> SleepEventMap;





};
