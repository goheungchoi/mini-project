#pragma once
#include "physx/PxPhysicsAPI.h"
#include "PhyjixUtil.h"
#include "IPhyjixWorld.h"

class RigidBody;
class IRigidBody;
class PhyjixEventHandler;
struct PhyjixRay;


class PhyjixWorld : public IPhyjixWorld
{
public:
  PhyjixWorld(physx::PxPhysics* physics,
              physx::PxDefaultCpuDispatcher* dispatcher);
  ~PhyjixWorld();

  void CreateDefaultGround() override;

  IRigidBody* AddRigidBody(const DirectX::SimpleMath::Vector3& position,
                           const DirectX::SimpleMath::Quaternion& Rotation,
                           const DirectX::SimpleMath::Vector3& offsetpos,
                           const DirectX::SimpleMath::Quaternion& offsetrot,
                           const DirectX::SimpleMath::Vector3& offsetsize,
                           ColliderShape shape, bool isStatic = false,
                           bool isKinematic = true) override;
  void RemoveRigidBody(IRigidBody* body) override;

  ICharacterController* CreateCharacterController(
      const DirectX::SimpleMath::Vector3& position, float radius, float height);
  void RemoveCharacterController(ICharacterController* controller);
  void UpdateCharacterControllers(float deltaTime);

  void Update(float deltaTime) override;
  IRigidBody* GetGroundActor() override;


  
  void CreateRay(DirectX::SimpleMath::Vector3 camerapos,
                       DirectX::SimpleMath::Vector2 mousepos,
                               DirectX::SimpleMath::Matrix view,
                               DirectX::SimpleMath::Matrix projection,
                               DirectX::SimpleMath::Vector2 ScreenSize) override;
  void CastRay() override;
  void UpdateRay(DirectX::SimpleMath::Vector3 camerapos, DirectX::SimpleMath::Vector2 mousepos, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection, DirectX::SimpleMath::Vector2 ScreenSize) override;
  void LeftClick() { isLClicked = true; }
  void RightClick() { isRClicked = true; }


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

  PhyjixRay* _ray = nullptr;
  
  BOOL isLClicked = false;
  BOOL isRClicked = false;

};
struct PhyjixRay
{
  physx::PxVec3 Pos{};
  physx::PxVec3 Direction{};
  DirectX::SimpleMath::Vector2 ScreenSize{};
  void UpdateRay(DirectX::SimpleMath::Vector3 camerapos,
                 DirectX::SimpleMath::Vector2 mousepos,
                 DirectX::SimpleMath::Matrix view,
                 DirectX::SimpleMath::Matrix projection)
  {
    DirectX::SimpleMath::Vector4 world = PhyjixUtil::NDCToWorld(
        PhyjixUtil::ScreenToNDC(mousepos.x, mousepos.y, ScreenSize.x,
                                ScreenSize.y),
        view, projection);

    Pos = physx::PxVec3(camerapos.x, camerapos.y, camerapos.z);
    DirectX::SimpleMath::Vector3 rayVec = DirectX::XMVector3Normalize(
        DirectX::SimpleMath::Vector3(world.x, world.y, world.z));
    Direction = physx::PxVec3(rayVec.x, rayVec.y, rayVec.z);
  }
};
