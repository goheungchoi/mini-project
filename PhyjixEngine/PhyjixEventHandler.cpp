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
  
}

void PhyjixEventHandler::onWake(physx::PxActor** pActor, physx::PxU32) {}

void PhyjixEventHandler::onSleep(physx::PxActor**, physx::PxU32) {}
