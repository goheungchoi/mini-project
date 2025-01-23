#pragma once
#include "physx/PxPhysicsAPI.h"
#include "IPhyjixWorld.h"

class RigidBody;
class IRigidBody;
class PhyjixEventHandler;

class PhyjixWorld : public IPhyjixWorld
{
public:
  PhyjixWorld(physx::PxPhysics* physics,
              physx::PxDefaultCpuDispatcher* dispatcher);
  ~PhyjixWorld();

  void CreateDefaultGround() override;

  IRigidBody* AddRigidBody(const DirectX::SimpleMath::Vector3& position,
                           const DirectX::SimpleMath::Vector3& size,
                           ColliderShape shape, bool isStatic = false,
                           bool isKinematic = true) override;
  void RemoveRigidBody(IRigidBody* body) override;

  ICharacterController* CreateCharacterController(
      const DirectX::SimpleMath::Vector3& position, float radius, float height);
  void RemoveCharacterController(ICharacterController* controller);
  void UpdateCharacterControllers(float deltaTime);

  void Update(float deltaTime) override;
  IRigidBody* GetGroundActor() override;

private:
  float elapsedTimer = 0.f;
  physx::PxScene* _scene = nullptr;
  physx::PxPhysics* _physics = nullptr;
  PhyjixEventHandler* _eventhandler = nullptr;

  physx::PxControllerManager* _controllerManager = nullptr;
  std::vector<ICharacterController*> _characterControllers;

  physx::PxPvdSceneClient* pvdClient = nullptr;
  RigidBody* groundrigidbody = nullptr;
  physx::PxRigidStatic* groundPlane = nullptr;

  physx::PxMaterial* mMaterial = nullptr;

};
