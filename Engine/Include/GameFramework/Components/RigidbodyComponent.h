#pragma once
#include "ComponentBase.h"
#include "TransformComponent.h"
#include "../../PhyjixEngine/RigidBody.h"

#include <directxtk/SimpleMath.h>

// component which controls the movement and simulate collision via physx;
// TransformComponent is effected and controlled by this component;
class RigidbodyComponent : public ComponentBase
{
  friend class TransformComponent;
  using Event = std::function<void(void)>;

public:

  //rigidbody initalize
  void Initialize(const DirectX::SimpleMath::Vector3& offsetTranslation,
                  const DirectX::SimpleMath::Quaternion& offsetQuaternion,
                  const DirectX::SimpleMath::Vector3& size,
                  ColliderShape cShape, BOOL isStatic, BOOL isKinematic,
                  IPhyjixWorld* world);

  //wrapper of the iRigidbody collision setting
  void SetCollisionEvent(IRigidBody* other, eCollisionEventType eventType, Event event);
  void RemoveCollisionEvent(IRigidBody* other);
  void SetOffsetTransform(const Vector3& position, const Quaternion& quaternion,
                          const Vector3& scale);


  RigidbodyComponent(class GameObject* owner) : ComponentBase(owner) {}
  ~RigidbodyComponent();

  void Translate(Vector3 force);
  void Rotate(Vector3 Torque);
  void SetKinematicTransform(Vector3 pos, Quaternion rot);
  void UpdateKinematicTransform();
  void SetWorldTransform(Vector3 pos, Quaternion rot);
  void ClearForce();
  void ClearTorque();
  void EnableCollision();
  void DisableCollision();
  void EnableGravity();
  void DisableGravity();
  void EnableSimulation();
  void DisableSimulation();

  IRigidBody* GetRigidBody() { return _rigidbody; };

  void UpdateFromTransform();
  void UpdateToTransform();

  void EnableDebugDraw();
  void DisableDebugDraw();
  BOOL GetDebugDrawFlag() { return _bDebugDrawFlag; };
  void UpdateDebugDrawMatrix();
  static float scalingFactor;
  Color debugColor = {1, 0, 1, 1};
  XMMATRIX debugDrawMatrix = Matrix::Identity;


	void RegisterRigidBodyToWorld();
  void UnregisterRigidBodyFromWorld();

	void BeginOverlap(RigidbodyComponent* other);
  void EndOverlap(RigidbodyComponent* other);
  bool IsOverlapping() { return bIsOverlapping; };
  void SetIsOverlapping(bool isoverlapping) {   bIsOverlapping = isoverlapping; };
  void Hover();
  void LeftClick();
  void RightClick();




  XMMATRIX offsetMatrix = Matrix::Identity;
  XMVECTOR offsetScale = {1,1,1};
  XMVECTOR offsetRotation = {0,0,0,1};
  XMVECTOR offsetTranslation = {0,0,0};

private:
  BOOL bIsOverlapping = false;
  BOOL isKinematic = false;
  BOOL isStatic = false;
  TransformComponent* GetTransformComponent();
  IRigidBody*   _rigidbody = nullptr;
  IPhyjixWorld* _world = nullptr;
  physx::PxTransform _prevTransform;

  BOOL _bDebugDrawFlag = false;

};

