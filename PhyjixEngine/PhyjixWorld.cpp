#include "pch.h"
#include "PhyjixWorld.h"

PhyjixWorld::PhyjixWorld(PxPhysics* physics, PxDefaultCpuDispatcher* dispatcher)
{
  PxSceneDesc sceneDesc(physics->getTolerancesScale());
  sceneDesc.cpuDispatcher = dispatcher;
  sceneDesc.gravity = PxVec3(0, -98.f, 0);
  sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

  _scene = physics->createScene(sceneDesc);
  _physics = physics;
  _controllerManager = PxCreateControllerManager(*_scene);

  pvdClient = _scene->getScenePvdClient();
  if (pvdClient)
  {
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS,
                               true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES,
                               true);
  }
  sceneDesc.filterShader =
      [](PxFilterObjectAttributes attributes0, PxFilterData filterData0,
         PxFilterObjectAttributes attributes1, PxFilterData filterData1,
         PxPairFlags& pairFlags, const void* constantBlock,
         PxU32 constantBlockSize) -> physx::PxFilterFlags {
    pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT |
                physx::PxPairFlag::eTRIGGER_DEFAULT |
                physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
    return physx::PxFilterFlag::eDEFAULT;
  };

  _eventhandler = new PhyjixEventHandler();
  sceneDesc.simulationEventCallback = _eventhandler;
}

PhyjixWorld::~PhyjixWorld()
{
  if (_scene)
    _scene->release();
  if (_physics)
    _physics->release();
  if (_controllerManager)
    _controllerManager->release();
}

IRigidBody* PhyjixWorld::AddRigidBody(
    const DirectX::SimpleMath::Vector3& position,
    const DirectX::SimpleMath::Vector3& size, ColliderShape cShape,
    bool isStatic, bool isKinematic)
{
  IRigidBody* physxbody = new RigidBody(_physics, position, size, cShape,
                                        isStatic, isKinematic, this);

  _scene->addActor(*(static_cast<RigidBody*>(physxbody)->_actor));
  return physxbody;
}

void PhyjixWorld::RemoveRigidBody(IRigidBody* body)
{
  auto* physxBody = static_cast<RigidBody*>(body);
  _scene->removeActor(*physxBody->_actor);
}

ICharacterController* PhyjixWorld::CreateCharacterController(
    const DirectX::SimpleMath::Vector3& position, float radius, float height)
{
  PxCapsuleControllerDesc desc;
  desc.position = PxExtendedVec3(position.x, position.y, position.z);
  desc.radius = radius;
  desc.height = height;
  desc.material = mMaterial;

  auto controller = new CharacterController();
  controller->Initialize(_controllerManager, desc);
  CharacterController* rawPtr = controller;
  _characterControllers.push_back(std::move(controller));
  return rawPtr;
}

void PhyjixWorld::RemoveCharacterController(ICharacterController* controller)
{
  auto it =
      std::find_if(_characterControllers.begin(), _characterControllers.end(),
                   [controller](const ICharacterController* ptr) {
                     return ptr == controller;
                   });

  if (it != _characterControllers.end())
  {
    _characterControllers.erase(it);
  }
}

void PhyjixWorld::UpdateCharacterControllers(float deltaTime)
{
  for (auto controls : _characterControllers)
  {
    controls->Update(deltaTime);
  }
}

void PhyjixWorld::Update(float deltaTime)
{
  UpdateCharacterControllers(deltaTime);
  _scene->simulate(deltaTime);
  _scene->fetchResults(true);

  //elapsedTimer += deltaTime;
  //if (elapsedTimer >= 0.0166f)
  //{
  //  elapsedTimer -= 0.0166f;
  //}
}

void PhyjixWorld::CreateDefaultGround()
{
  mMaterial = _physics->createMaterial(0.5f, 0.5f, 0.f);
  groundPlane =
      PxCreatePlane(*_physics, physx::PxPlane(0, 1, 0, 50), *mMaterial);
  _scene->addActor(*groundPlane);
}
