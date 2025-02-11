#pragma once
#include "physx/PxPhysicsAPI.h"
#include "IRigidBody.h"
#include "ICollisionEvent.h"

class PhyjixWorld;
class RigidBody : public IRigidBody, public ICollisionEvent
{
  using CollisionEvent = std::function<void(RigidBody* self, RigidBody* other)>;

public:
   RigidBody(physx::PxPhysics* physics, const DirectX::SimpleMath::Vector3& position,
            const DirectX::SimpleMath::Quaternion& rotation,
                     const DirectX::SimpleMath::Vector3& offsetpos,
            const DirectX::SimpleMath::Quaternion& offsetrot,
            const DirectX::SimpleMath::Vector3& offsetsize, ColliderShape shape,
            BOOL isStatic, BOOL isKinematic, PhyjixWorld* world);
  ~RigidBody();

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
  void KinematicMoveTo(DirectX::SimpleMath::Vector3 pos,
                       DirectX::SimpleMath::Vector4 rot) override;
  bool isKinematic() override { return bIsKinematic; };



  void EnableCollision() override;
  void DisableCollision() override;
  void EnableGravity() override;
  void DisableGravity() override;

  void EnableSimulation() override;
  void DisableSimulation() override;

  void EnableSceneQuery() override;
  void DisableSceneQuery() override;

  void WakeUp() override;
  void Sleep() override;


  DirectX::SimpleMath::Vector3 GetWorldPosition() override;
  DirectX::SimpleMath::Vector4 GetWorldRotation() override;
  physx::PxTransform GetWorldTransform() override;

  void SetWorldTransform(DirectX::SimpleMath::Vector3 pos,
                         DirectX::SimpleMath::Vector4 rot) override;


  ColliderShape GetColliderShapeType() override;

  // ICollisionEvent을(를) 통해 상속됨
  void OnCollisionEnter(IRigidBody* other) override;
  void OnCollisionExit(IRigidBody* other) override;
  void OnTrigger(IRigidBody* other) override;
  void OnWake() override;
  void OnSleep() override;
  void OnHover() override;
  void OnLeftClick() override;
  void OnRightClick() override;
  void OnOverlapBegin(IRigidBody* other) override;
  void OnOverlapEnd(IRigidBody* other) override;

  physx::PxRigidActor* _actor = nullptr;

  physx::PxRigidActor* GetActor();


private:
  bool isStatic = false;
  bool bIsKinematic = false;
  PhyjixWorld* _world = nullptr;
  physx::PxShape* _defaultShape = nullptr;
  physx::PxShape* _triggerShape = nullptr;
  ColliderShape shape;


  std::unordered_map<IRigidBody*, std::function<void(void)>> CollisionEnterEventMap;
  std::unordered_map<IRigidBody*, std::function<void(void)>> CollisionExitEventMap;

  std::unordered_map<IRigidBody*, std::function<void(void)>> TriggerEventMap;
  std::vector<std::function<void(void)>> WakeEventMap;
  std::vector<std::function<void(void)>> SleepEventMap;
  std::vector<std::function<void(void)>> HoverEventMap;
  std::vector<std::function<void(void)>> LClickEventMap;
  std::vector<std::function<void(void)>> RClickEventMap;
  std::unordered_map<IRigidBody*, std::function<void(void)>>
      OverlapBeginEventMap;
  std::unordered_map<IRigidBody*, std::function<void(void)>> OverlapEndEventMap;

  physx::PxRigidDynamic* GetDynamicActor();

  // Inherited via IRigidBody
  void RemoveCollisionEvent(IRigidBody* other) override;
};

