#include "pch.h"
#include "PhyjixWorld.h"
bool collisionMatrix[4][4] = {{true, true, true, true},
                              {true, true, true, true},
                              {true, true, true, true},
                              {true, true, true, true}};



PhyjixWorld::PhyjixWorld(physx::PxPhysics* physics, physx::PxDefaultCpuDispatcher* dispatcher)
{
  physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
  sceneDesc.cpuDispatcher = dispatcher;
  sceneDesc.gravity = physx::PxVec3(0, -98.f, 0);
  //sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD; // 고속 이동 대비

  sceneDesc.kineKineFilteringMode = physx::PxPairFilteringMode::eKEEP;

 sceneDesc.filterShader =
      [](physx::PxFilterObjectAttributes at0, physx::PxFilterData fd0,
         physx::PxFilterObjectAttributes at1, physx::PxFilterData fd1,
         physx::PxPairFlags& pairFlags, const void* constantBlock,
         physx::PxU32 constantBlockSize) -> physx::PxFilterFlags
  {
         
    if ((physx::PxFilterObjectIsTrigger(at0) ||
         physx::PxFilterObjectIsTrigger(at1)))
    {
      pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
      return physx::PxFilterFlag::eDEFAULT;
    }
    return physx::PxDefaultSimulationFilterShader(
        at0, fd0, at1, fd1, pairFlags, constantBlock, constantBlockSize);
  };

  _eventhandler = new PhyjixEventHandler();
  sceneDesc.simulationEventCallback = _eventhandler;

  

  sceneDesc.kineKineFilteringMode = physx::PxPairFilteringMode::eKEEP;


  _scene = physics->createScene(sceneDesc);
  _physics = physics;
  _controllerManager = PxCreateControllerManager(*_scene);

  pvdClient = _scene->getScenePvdClient();
  if (pvdClient)
  {

    _scene->setVisualizationParameter(
        physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS,
                               true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES,
                               true);
  }
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
    const DirectX::SimpleMath::Quaternion& rotation,
    const DirectX::SimpleMath::Vector3& offsetpos,
    const DirectX::SimpleMath::Quaternion& offsetrot,
    const DirectX::SimpleMath::Vector3& offsetsize,
    ColliderShape cShape,
    bool isStatic, bool isKinematic)
{
  IRigidBody* physxbody = new RigidBody(_physics, position, rotation, offsetpos, offsetrot, offsetsize,
                                        cShape, isStatic, isKinematic, this);

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
  physx::PxCapsuleControllerDesc desc;
  desc.position = physx::PxExtendedVec3(position.x, position.y, position.z);
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
  isLClicked = false;
  isRClicked = false;
  //UpdateCharacterControllers(deltaTime);
  elapsedTimer += deltaTime;
  if (elapsedTimer >= 0.0166f)
  {
   _scene->simulate(deltaTime);
  _scene->fetchResults(true);
    elapsedTimer -= 0.0166f;
  }
}

IRigidBody* PhyjixWorld::GetGroundActor()
{
  return static_cast<IRigidBody*>(groundrigidbody);
}


// mousepos = 마우스 좌표 / view, projection = camera view, projection 매트릭스 /  pos =  camera position
void PhyjixWorld::CreateRay(DirectX::SimpleMath::Vector3 camerapos,
    DirectX::SimpleMath::Vector2 mousepos, DirectX::SimpleMath::Matrix view,
    DirectX::SimpleMath::Matrix projection,
    DirectX::SimpleMath::Vector2 ScreenSize)
{


  DirectX::SimpleMath::Vector4 world = PhyjixUtil::NDCToWorld(
      PhyjixUtil::ScreenToNDC(mousepos.x, mousepos.y, ScreenSize.x,
                              ScreenSize.y),view, projection);


  _ray = new PhyjixRay;
  _ray->Pos = physx::PxVec3(camerapos.x, camerapos.y, camerapos.z);
  DirectX::SimpleMath::Vector3 rayVec = DirectX::XMVector3Normalize(DirectX::SimpleMath::Vector3(world.x, world.y, world.z));
  _ray->Direction = physx::PxVec3(rayVec.x, rayVec.y, rayVec.z);
}

void PhyjixWorld::CastRay()
{
  physx::PxRaycastBuffer hitBuffer;
  float maxDistance = 1000.f;

  if (_scene->raycast(_ray->Pos, _ray->Direction, maxDistance, hitBuffer))
  {
    if (hitBuffer.hasBlock)
    {
      const physx::PxRaycastHit& hit = hitBuffer.block;
      physx::PxRigidActor* actor = hit.actor;
      if (actor)
      {
        ICollisionEvent* collisionActor =
            static_cast<RigidBody*>(actor->userData);

        if (isLClicked || isRClicked)
        {
          if (isLClicked)
            collisionActor->OnLeftClick();
          if (isRClicked)
            collisionActor->OnRightClick();
        }
        else
          collisionActor->OnHover();
      }
    }
  }
  
}

void PhyjixWorld::UpdateRay(DirectX::SimpleMath::Vector3 camerapos,
    DirectX::SimpleMath::Vector2 mousepos, DirectX::SimpleMath::Matrix view,
    DirectX::SimpleMath::Matrix projection,
    DirectX::SimpleMath::Vector2 ScreenSize)
{
  _ray->ScreenSize = ScreenSize;
  _ray->UpdateRay(camerapos, mousepos, view, projection);
}


void PhyjixWorld::CreateDefaultGround()
{
  mMaterial = _physics->createMaterial(0.5f, 0.5f, 0.f);
  groundPlane =
      PxCreatePlane(*_physics, physx::PxPlane(0, 1, 0, 0.f), *mMaterial);
  groundrigidbody =
      new RigidBody(_physics, {0, 0, 0}, {0, 0, 0, 1}, {0, 0, 0}, {0, 0, 0, 1},
                    {1, 1, 1},
                    ColliderShape::eCubeCollider, true, false, this);
  groundPlane->userData = groundrigidbody;
  groundrigidbody->_actor = groundPlane;
  _scene->addActor(*groundrigidbody->_actor);

}
