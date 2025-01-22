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

  shape = cShape;
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

//PxActor* RigidBody::GetActor() const
//{
//  return _actor;
//}

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

PxRigidDynamic* RigidBody::GetDynamicActor()
{
  return static_cast<PxRigidDynamic*>(_actor);
}

void RigidBody::SetCollisionEvent(eCollisionEventType collisiontype, IRigidBody* other, std::function<void(void)> event)
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

void RigidBody::SetLinVelocity(DirectX::SimpleMath::Vector3 vel)
{
  GetDynamicActor()->setLinearVelocity(PhyjixUtil::VecToPxVec(vel));
}

void RigidBody::SetMaxLinVelocity(float vel)
{
  GetDynamicActor()->setMaxLinearVelocity(vel);
}

void RigidBody::SetAngVelocity(DirectX::SimpleMath::Vector3 vel)
{
  GetDynamicActor()->setAngularVelocity(PhyjixUtil::VecToPxVec(vel));
}

void RigidBody::SetMaxAngVelocity(float vel)
{
  GetDynamicActor()->setMaxAngularVelocity(vel);
}

DirectX::SimpleMath::Vector3 RigidBody::GetLinVelocity()
{
  return PhyjixUtil::PxVecToVec(GetDynamicActor()->getLinearVelocity());
}

float RigidBody::GetMaxLinVelocity()
{
  return GetDynamicActor()->getMaxLinearVelocity();
  
}

DirectX::SimpleMath::Vector3 RigidBody::GetAngVelocity()
{
  return PhyjixUtil::PxVecToVec(GetDynamicActor()->getAngularVelocity());
  
}

float RigidBody::GetMaxAngVelocity()
{
  return GetDynamicActor()->getMaxAngularVelocity();
}

void RigidBody::EnableCollision()
{
  PxShape* shape = nullptr;
  PxU32 shapeCount = GetDynamicActor()->getNbShapes();
  if (shapeCount > 0)
  {
    GetDynamicActor()->getShapes(&shape, 1, 0);
    if (shape)
      shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
  }
}

void RigidBody::DisableCollision()
{
  PxShape* shape = nullptr;
  PxU32 shapeCount = GetDynamicActor()->getNbShapes();
  if (shapeCount > 0)
  {
    GetDynamicActor()->getShapes(&shape, 1, 0);
    if (shape)
      shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
  }
}

void RigidBody::EnableGravity()
{
  _actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
}

void RigidBody::DisableGravity()
{
  _actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
}

void RigidBody::WakeUp()
{
  GetDynamicActor()->wakeUp();
}

void RigidBody::Sleep()
{
  GetDynamicActor()->putToSleep();
}

ColliderShape RigidBody::GetColliderShapeType()
{
  return shape;
  
}
