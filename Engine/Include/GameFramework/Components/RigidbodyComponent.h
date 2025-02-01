#pragma once
#include "ComponentBase.h"
#include "TransformComponent.h"
#include "../../PhyjixEngine/RigidBody.h"

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
  void SetOffsetTransform(const Vector3& position, const Quaternion& quaternion,
                          const Vector3& scale);


  RigidbodyComponent(class GameObject* owner) : ComponentBase(owner) {}
  ~RigidbodyComponent();

  void Translate(Vector3 force);
  void Rotate(Vector3 Torque);
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
#ifdef _DEBUG
  void EnableDebugDraw();
  void DisableDebugDraw();
  BOOL GetDebugDrawFlag() { return _bDebugDrawFlag; };
  void UpdateDebugDrawMatrix();
  static float scalingFactor;
#endif

	void RegisterRigidBodyToWorld();
  void UnregisterRigidBodyFromWorld();

	void BeginOverlap(RigidbodyComponent* other);
  void EndOverlap(RigidbodyComponent* other);



  XMMATRIX offsetMatrix = Matrix::Identity;
  XMMATRIX debugDrawMatrix = Matrix::Identity;
  XMVECTOR offsetScale = {1,1,1};
  XMVECTOR offsetRotation = {0,0,0,1};
  XMVECTOR offsetTranslation = {0,0,0};

private:

  TransformComponent* GetTransformComponent();
  IRigidBody* _rigidbody = nullptr;
  IPhyjixWorld* _world = nullptr;
  physx::PxTransform _prevTransform;


#ifdef _DEBUG
  BOOL _bDebugDrawFlag = false;
#endif
};

