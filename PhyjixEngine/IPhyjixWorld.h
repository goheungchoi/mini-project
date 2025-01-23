#pragma once
class IRigidBody;
class ICharacterController;

enum class ColliderShape
{
  eCubeCollider,
  eSphereCollider,
  eCapsuleCollider
};
struct PhyjixRay;
class IPhyjixWorld
{
public:
  virtual ~IPhyjixWorld() = default;

      // object managing
  virtual IRigidBody* AddRigidBody(const DirectX::SimpleMath::Vector3& position,
                                   const DirectX::SimpleMath::Vector3& size,
                                   ColliderShape shape, bool isStatic = false,
                                   bool isKinematic = true) = 0;
  virtual void RemoveRigidBody(IRigidBody* body) = 0;

  virtual ICharacterController* CreateCharacterController(
      const DirectX::SimpleMath::Vector3& position, float radius,
      float height) = 0;
  virtual void RemoveCharacterController(ICharacterController* controller) = 0;
  virtual void UpdateCharacterControllers(float deltaTime) = 0;

  virtual void CreateDefaultGround() = 0;
  virtual IRigidBody* GetGroundActor() = 0;
  // physics update
  virtual void Update(float deltaTime) = 0;

  virtual void CastRay(PhyjixRay* ray) = 0;
  virtual PhyjixRay* CreateRay(DirectX::SimpleMath::Vector3 pos,
                               DirectX::SimpleMath::Vector2 mousepos,
                              DirectX::SimpleMath::Matrix view,
                              DirectX::SimpleMath::Matrix projection,
                              DirectX::SimpleMath::Vector2 ScreenSize) = 0;

  virtual void LeftClick() = 0;
  virtual void RightClick() = 0;
};
