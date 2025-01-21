#include "pch.h"
#include "RigidBody.h"

RigidBody::RigidBody(PxPhysics* physics,
                     const DirectX::SimpleMath::Vector3& position,
                     const DirectX::SimpleMath::Vector3& size,
                     ColliderShape cShape, BOOL isStatic, BOOL isKinematic,
                     PhyjixWorld* world)
{
  _world = world;
  PxTransform transform(PhyjixUtil::VecToPxVec(position));

  if (!isStatic)
  {
    _actor = physics->createRigidDynamic(transform);
    if (isKinematic)
      static_cast<PxRigidDynamic*>(_actor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
  }
  else
    _actor = physics->createRigidStatic(transform);


  switch (cShape)
  {
  case ColliderShape::eCubeCollider:
    _defaultShape =
        physics->createShape(PxBoxGeometry(size.x, size.y, size.z),
                             *physics->createMaterial(0.5f, 0.5f, 0.6f));
    _nonElasticShape =
        physics->createShape(PxBoxGeometry(size.x, size.y, size.z),
                             *physics->createMaterial(0.5f, 0.5f, 0.0f));
    break;

  case ColliderShape::eSphereCollider:
    _defaultShape = physics->createShape(
        PxSphereGeometry(size.x), *physics->createMaterial(0.5f, 0.5f, 0.6f));
    _nonElasticShape = physics->createShape(
        PxSphereGeometry(size.x), *physics->createMaterial(0.5f, 0.5f, 0.6f));
    break;

  case ColliderShape::eCapsuleCollider:
    _defaultShape =
        physics->createShape(PxCapsuleGeometry(size.x, size.y),
                             *physics->createMaterial(0.5f, 0.5f, 0.6f));
    _nonElasticShape =
        physics->createShape(PxCapsuleGeometry(size.x, size.y),
                             *physics->createMaterial(0.5f, 0.5f, 0.6f));
    break;
  }
  _actor->attachShape(*_defaultShape);


  _actor->userData = this;

}

RigidBody::~RigidBody()
{
  if (_defaultShape)
    _defaultShape->release();
  if (_nonElasticShape)
    _nonElasticShape->release();
  if (_actor)
    _actor->release();
}

void RigidBody::SetPosition(const DirectX::SimpleMath::Vector3& position)
{
  _actor->setGlobalPose(PxTransform(PhyjixUtil::VecToPxVec(position)));
}

DirectX::SimpleMath::Vector3 RigidBody::GetPosition() const
{
  return PhyjixUtil::PxVecToVec(_actor->getGlobalPose().p);
}

void RigidBody::SetRotation(const DirectX::SimpleMath::Vector4& rotation)
{
  _actor->setGlobalPose(PxTransform(rotation.x, rotation.x, rotation.x,
                                    PxQuatT<float>(rotation.w)));
}

DirectX::SimpleMath::Vector4 RigidBody::GetRotation() const
{
  return DirectX::SimpleMath::Vector4(
      _actor->getGlobalPose().q.x, _actor->getGlobalPose().q.y,
      _actor->getGlobalPose().q.z, _actor->getGlobalPose().q.w);
}

void RigidBody::ApplyForce(const DirectX::SimpleMath::Vector3& force)
{
  static_cast<PxRigidDynamic*>(_actor)->addForce(PhyjixUtil::VecToPxVec(force));
}

PxRigidActor* RigidBody::GetActor() const
{
  return _actor;
}

void RigidBody::OnCollisionEnter(IRigidBody* other) 
{
  if (CollisionEnterEventMap.find(other) != CollisionEnterEventMap.end())
    CollisionEnterEventMap[other]();
}

void RigidBody::OnCollisionExit(IRigidBody* other) 
{
  if (CollisionExitEventMap.find(other) != CollisionExitEventMap.end())
    CollisionExitEventMap[other]();
}

void RigidBody::OnTrigger(IRigidBody* other) 
{
  if (TriggerEventMap.find(other) != TriggerEventMap.end())
    TriggerEventMap[other]();
}

void RigidBody::OnWake() 
{
  if (!WakeEventMap.empty())
  {
    for (auto event : WakeEventMap)
      event();
  }
}

void RigidBody::OnSleep() 
{
  if (!SleepEventMap.empty())
  {
    for (auto event : SleepEventMap)
      event();
  }
}

void RigidBody::SetCollisionEvent(eCollisionEventType collisiontype,
                                  IRigidBody* other,
                                  std::function<void(void)> event)
{
    switch (collisiontype)
    {
    case eCollisionEventType::eCollisionEnter:
      CollisionEnterEventMap.insert({other, event});
      break;
    case eCollisionEventType::eCollisionExit:
      CollisionExitEventMap.insert({other, event});
      break;
    case eCollisionEventType::eTrigger:
      TriggerEventMap.insert({other, event});
      break;
    case eCollisionEventType::eWake:
      WakeEventMap.push_back(event);
      break;
    case eCollisionEventType::eSleep:
      SleepEventMap.push_back(event);
      break;
  }
}
