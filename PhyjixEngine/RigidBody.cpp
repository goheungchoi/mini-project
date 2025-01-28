#include "pch.h"
#include "RigidBody.h"
RigidBody::RigidBody(physx::PxPhysics* physics,
                     const DirectX::SimpleMath::Vector3& position,
                     const DirectX::SimpleMath::Vector3& size,
                     ColliderShape cShape, BOOL isStatic, BOOL isKinematic,
                     PhyjixWorld* world)
{
  _world = world;
  physx::PxTransform transform(PhyjixUtil::VecToPxVec(position));
  isStatic = isStatic;
  if (!isStatic)
  {
    _actor = physics->createRigidDynamic(transform);
    if (isKinematic)
      static_cast<physx::PxRigidDynamic*>(_actor)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
  }
  else
    _actor = physics->createRigidStatic(transform);

  shape = cShape;
  physx::PxMaterial* material = physics->createMaterial(0.5f, 0.5f, 1.f);
  switch (cShape)
  {
  case ColliderShape::eCubeCollider:
    _defaultShape =
        physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z),
                             *material);
    _nonElasticShape =
        physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z),
                             *material);
    break;

  case ColliderShape::eSphereCollider:
    _defaultShape = physics->createShape(
        physx::PxSphereGeometry(size.x), *physics->createMaterial(0.5f, 0.5f, 0.6f));
    _nonElasticShape = physics->createShape(
        physx::PxSphereGeometry(size.x), *physics->createMaterial(0.5f, 0.5f, 0.6f));
    break;

  case ColliderShape::eCapsuleCollider:
    _defaultShape =
        physics->createShape(physx::PxCapsuleGeometry(size.x, size.y),
                             *physics->createMaterial(0.5f, 0.5f, 0.6f));
    _nonElasticShape =
        physics->createShape(physx::PxCapsuleGeometry(size.x, size.y),
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
    for (auto event : SleepEventMap)
      event();
}

void RigidBody::OnHover()
{
  if (!HoverEventMap.empty())
    for (auto event : HoverEventMap)
      event();
}

void RigidBody::OnLeftClick()
{
  if (!LClickEventMap.empty())
    for (auto event : LClickEventMap)
      event();
}

void RigidBody::OnRightClick()
{
  if (!RClickEventMap.empty())
    for (auto event : RClickEventMap)
      event();
}

physx::PxRigidDynamic* RigidBody::GetDynamicActor()
{
  return static_cast<physx::PxRigidDynamic*>(_actor);
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
    case eCollisionEventType::eHover:
      HoverEventMap.push_back(event);
      break;
    case eCollisionEventType::eLClick:
      LClickEventMap.push_back(event);
      break;
    case eCollisionEventType::eRClick:
      RClickEventMap.push_back(event);
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
  physx::PxShape* shape = nullptr;
  physx::PxU32 shapeCount = GetDynamicActor()->getNbShapes();
  if (shapeCount > 0)
  {
    GetDynamicActor()->getShapes(&shape, 1, 0);
    if (shape)
      shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
  }
}

void RigidBody::DisableCollision()
{
  _actor->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);
  physx::PxShape* shape = nullptr;
  physx::PxU32 shapeCount = GetDynamicActor()->getNbShapes();
  if (shapeCount > 0)
  {
    GetDynamicActor()->getShapes(&shape, 1, 0);
    if (shape)
      shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);

  }
}

void RigidBody::EnableGravity()
{
  _actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
}

void RigidBody::DisableGravity()
{
  _actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
}

void RigidBody::WakeUp()
{
  GetDynamicActor()->wakeUp();
}

void RigidBody::Sleep()
{
  GetDynamicActor()->putToSleep();
}

DirectX::SimpleMath::Vector3 RigidBody::GetWorldPosition()
{
  if (isStatic)
    return PhyjixUtil::PxVecToVec(
        static_cast<physx::PxRigidStatic*>(_actor)->getGlobalPose().p);
  else
    return PhyjixUtil::PxVecToVec(GetDynamicActor()->getGlobalPose().p);

}

DirectX::SimpleMath::Vector4 RigidBody::GetWorldRotation()
{
  if (isStatic)
      return PhyjixUtil::PxQuatToVec(static_cast<physx::PxRigidStatic*>(_actor)->getGlobalPose().q);
  else 
      return PhyjixUtil::PxQuatToVec(GetDynamicActor()->getGlobalPose().q);
}

physx::PxTransform RigidBody::GetWorldTransform()
{
  if (isStatic)
    return static_cast<physx::PxRigidStatic*>(_actor)->getGlobalPose();
  else
    return GetDynamicActor()->getGlobalPose();
}
  void RigidBody::SetWorldTransform(DirectX::SimpleMath::Vector3 pos,
                                  DirectX::SimpleMath::Vector4 rot)
{
  if (isStatic)
    static_cast<physx::PxRigidStatic*>(_actor)->setGlobalPose(physx::PxTransform(pos.x,pos.y,pos.z, physx::PxQuat(rot.x,rot.y,rot.z,rot.w)) );
  else
    GetDynamicActor()->setGlobalPose(physx::PxTransform(
        pos.x, pos.y, pos.z, physx::PxQuat(rot.x, rot.y, rot.z, rot.w)));
}


ColliderShape RigidBody::GetColliderShapeType()
{
  return shape;
  
}
