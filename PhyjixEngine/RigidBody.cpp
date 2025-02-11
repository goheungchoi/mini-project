#include "pch.h"
#include "RigidBody.h"
RigidBody::RigidBody(physx::PxPhysics* physics,
                      const DirectX::SimpleMath::Vector3& position,
                     const DirectX::SimpleMath::Quaternion& rotation,
                     const DirectX::SimpleMath::Vector3& offsetpos,
                     const DirectX::SimpleMath::Quaternion& offsetrot,
                     const DirectX::SimpleMath::Vector3& offsetsize,
                     ColliderShape cShape, BOOL _isStatic, BOOL isKinematic,
                     PhyjixWorld* world)
{
  _world = world;
  physx::PxTransform transform(PhyjixUtil::VecToPxVec(position),
                               PhyjixUtil::VecToPxQuat(rotation));
  isStatic = _isStatic;
  bIsKinematic = isKinematic;
  if (!isStatic)
  {
    _actor = physics->createRigidDynamic(transform);
    //GetDynamicActor()->setMass(1.f);
    GetDynamicActor()->setRigidBodyFlag(
        physx::PxRigidBodyFlag::eUSE_KINEMATIC_TARGET_FOR_SCENE_QUERIES, true);
    if (isKinematic)
      GetDynamicActor()->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC,
                                          true);

  }
  else
    _actor = physics->createRigidStatic(transform);
  shape = cShape;

  physx::PxMaterial* material = physics->createMaterial(0.5f, 0.5f, 0.f);
  switch (cShape)
  {
  case ColliderShape::eCubeCollider:
    
    _defaultShape = physx::PxRigidActorExt::createExclusiveShape(
        *_actor, physx::PxBoxGeometry(offsetsize.x, offsetsize.y, offsetsize.z),
        *material);
    _triggerShape = physx::PxRigidActorExt::createExclusiveShape(
        *_actor, physx::PxBoxGeometry(offsetsize.x, offsetsize.y, offsetsize.z),
        *material);

    break;

  case ColliderShape::eSphereCollider:
    _defaultShape = physx::PxRigidActorExt::createExclusiveShape(
        *_actor, physx::PxSphereGeometry(offsetsize.x), *material);
    _triggerShape = physx::PxRigidActorExt::createExclusiveShape(
        *_actor, physx::PxSphereGeometry(offsetsize.x), *material);
    break;

  case ColliderShape::eCapsuleCollider:
    _defaultShape = physics->createShape(
        physx::PxCapsuleGeometry(offsetsize.x, offsetsize.y),
                             *physics->createMaterial(0.5f, 0.5f, 0.6f));
    break;
  }
  _defaultShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
  _triggerShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
  if (isKinematic)
  {
    _defaultShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
    _defaultShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
  }
  
  //_actor->attachShape(*_defaultShape);
  _defaultShape->setLocalPose(physx::PxTransform(
      PhyjixUtil::VecToPxVec(offsetpos), PhyjixUtil::VecToPxQuat(offsetrot)));
  _triggerShape->setLocalPose(physx::PxTransform(
      PhyjixUtil::VecToPxVec(offsetpos), PhyjixUtil::VecToPxQuat(offsetrot)));

  _actor->userData = this;
}

RigidBody::~RigidBody()
{
  _world->RemoveRigidBody(this);
  if (_defaultShape)
  {
    _actor->detachShape(*_defaultShape);
    _defaultShape = nullptr;
  }
  if (_triggerShape)
  {
    _actor->detachShape(*_triggerShape);
    _triggerShape = nullptr;
  }



 
  if (!_actor->getScene())
  {
    _actor->release();
    _actor = nullptr;
  }
  _world = nullptr;
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

void RigidBody::OnOverlapBegin(IRigidBody* other)
{
  if (OverlapBeginEventMap.find(other) != OverlapBeginEventMap.end())
    OverlapBeginEventMap[other]();
}

void RigidBody::OnOverlapEnd(IRigidBody* other)
{
  if (OverlapEndEventMap.find(other) != OverlapEndEventMap.end())
    OverlapEndEventMap[other]();
}

physx::PxRigidActor* RigidBody::GetActor()
{
  return _actor;
}

physx::PxRigidDynamic* RigidBody::GetDynamicActor()
{
  return static_cast<physx::PxRigidDynamic*>(_actor);
}

void RigidBody::RemoveCollisionEvent(IRigidBody* other) 
{
  OverlapBeginEventMap.erase(other);
  OverlapEndEventMap.erase(other);
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
    case eCollisionEventType::eOverlapBegin:
      OverlapBeginEventMap.insert({other, event});
      break;
    case eCollisionEventType::eOverlapEnd:
      OverlapEndEventMap.insert({other, event});
      break;
    default:
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

void RigidBody::KinematicMoveTo(DirectX::SimpleMath::Vector3 pos,
                                DirectX::SimpleMath::Vector4 rot)
{
  GetDynamicActor()->setKinematicTarget(physx::PxTransform(
      PhyjixUtil::VecToPxVec(pos), PhyjixUtil::VecToPxQuat(rot)));

}

void RigidBody::EnableCollision()
{
  _defaultShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
  _defaultShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
}

void RigidBody::DisableCollision()
{
  _defaultShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
  _defaultShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
}

void RigidBody::EnableGravity()
{
  _actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
}

void RigidBody::DisableGravity()
{
  _actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
}

void RigidBody::EnableSimulation()
{
    if (isStatic)
      return;
    _triggerShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
    _defaultShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

}

void RigidBody::DisableSimulation()
{
  if (isStatic)
    return;
    _defaultShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
  _triggerShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
  
}

void RigidBody::EnableSceneQuery()
{

  _defaultShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
  _triggerShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
}

void RigidBody::DisableSceneQuery()
{
  _defaultShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, false);
  _triggerShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, false);
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
    return PhyjixUtil::PxQuatToVec(
        static_cast<physx::PxRigidStatic*>(_actor)->getGlobalPose().q);
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
