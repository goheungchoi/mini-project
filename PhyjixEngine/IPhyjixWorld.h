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
  virtual IRigidBody* AddRigidBody(
      const DirectX::SimpleMath::Vector3& position,
      const DirectX::SimpleMath::Quaternion& Rotation,
      const DirectX::SimpleMath::Vector3& offsetpos,
      const DirectX::SimpleMath::Quaternion& offsetrot,
      const DirectX::SimpleMath::Vector3& offsetsize, ColliderShape shape,
      bool isStatic = false, bool isKinematic = true) = 0;

  virtual void RemoveRigidBody(IRigidBody* body) = 0;

  virtual void CreateDefaultGround() = 0;
  virtual IRigidBody* GetGroundActor() = 0;
  // physics update
  virtual void Update(float deltaTime) = 0;

  virtual void CastRay() = 0;
  virtual void CreateRay(DirectX::SimpleMath::Vector3 camerapos,
                               DirectX::SimpleMath::Vector2 mousepos,
                              DirectX::SimpleMath::Matrix view,
                              DirectX::SimpleMath::Matrix projection,
                              DirectX::SimpleMath::Vector2 ScreenSize) = 0;
  virtual void UpdateRay(DirectX::SimpleMath::Vector3 camerapos,
                         DirectX::SimpleMath::Vector2 mousepos,
                         DirectX::SimpleMath::Matrix view,
                         DirectX::SimpleMath::Matrix projection,
                         DirectX::SimpleMath::Vector2 ScreenSize) = 0;
  virtual void LeftClick() = 0;
  virtual void RightClick() = 0;
};
