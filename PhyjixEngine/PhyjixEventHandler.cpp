#include "pch.h"
#include "PhyjixEventHandler.h"
void PhyjixEventHandler::onContact(const physx::PxContactPairHeader& pairHeader,
                                   const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{

  if (pairHeader.flags & physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_0)
  {
    printf("Actor 0 was removed during the collision.\n");
    return;
  }
  if (pairHeader.flags & physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_1)
  {
    printf("Actor 1 was removed during the collision.\n");
    return;
  }
  for (physx::PxU32 i = 0; i < nbPairs; i++)
  {
    const physx::PxContactPair& cp = pairs[i];
    ICollisionEvent* actor0 =
        static_cast<RigidBody*>(pairHeader.actors[0]->userData);
    ICollisionEvent* actor1 =
        static_cast<RigidBody*>(pairHeader.actors[1]->userData);

    IRigidBody* rigidbody0 =
        static_cast<RigidBody*>(pairHeader.actors[0]->userData);
    IRigidBody* rigidbody1 =
        static_cast<RigidBody*>(pairHeader.actors[1]->userData);

    

    if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND )
    {
      actor0->OnCollisionEnter(rigidbody1);
      actor1->OnCollisionEnter(rigidbody0);
    }
    if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
    {
      actor0->OnCollisionExit(rigidbody1);
      actor1->OnCollisionExit(rigidbody0); 
    }
  }

}

void PhyjixEventHandler::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
  for (physx::PxU32 i = 0; i < count; i++)
  {
    
    physx::PxTriggerPair& pair = pairs[i];

    if (pair.flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER |
                      physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER))
      continue;

    RigidBody* actor0 =
        static_cast<RigidBody*>(pair.triggerActor->userData);
    RigidBody* actor1 =
        static_cast<RigidBody*>(pair.otherActor->userData);


    if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
    {
      actor0->OnOverlapBegin(actor1);
      actor1->OnOverlapBegin(actor0);
    }
    else if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
    {
      actor0->OnOverlapEnd(actor1);
      actor1->OnOverlapEnd(actor0);
    }
  }
}

void PhyjixEventHandler::onWake(physx::PxActor** pActor, physx::PxU32) {}

void PhyjixEventHandler::onSleep(physx::PxActor**, physx::PxU32) {}
