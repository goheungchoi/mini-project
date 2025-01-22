#pragma once
#include "physx/PxPhysics.h"
#include "physx/PxPhysicsAPI.h"
using namespace physx;

#include "IPhyjixWorld.h"
class IRigidBody;
class PhyjixEventHandler;
class PhyjixWorld : public IPhyjixWorld
{
public:
  PhyjixWorld(PxPhysics* physics, PxDefaultCpuDispatcher* dispatcher);
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

private:
  float elapsedTimer = 0.f;
  PxScene* _scene = nullptr;
  PxPhysics* _physics = nullptr;
  PhyjixEventHandler* _eventhandler = nullptr;

  PxControllerManager* _controllerManager = nullptr;
  std::vector<ICharacterController*> _characterControllers;

  PxPvdSceneClient* pvdClient = nullptr;
  PxRigidStatic* groundPlane = nullptr;
  PxMaterial* mMaterial = nullptr;

  PxFilterFlags _customfiltershader;
};
