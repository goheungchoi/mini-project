#include "pch.h"
#include "PhyjixEventHandler.h"

void PhyjixEventHandler::onContact(const PxContactPairHeader& pairHeader,
                                   const PxContactPair* pairs, PxU32 nbPairs)
{
  for (PxU32 i = 0; i < nbPairs; i++)
  {
    const PxContactPair& cp = pairs[i];
    ICollisionEvent* actor0 =
        static_cast<ICollisionEvent*>(pairHeader.actors[0]->userData);
    ICollisionEvent* actor1 =
        static_cast<ICollisionEvent*>(pairHeader.actors[1]->userData);
    IRigidBody* rigidbody0 =
        static_cast<IRigidBody*>(pairHeader.actors[0]->userData);
    IRigidBody* rigidbody1 =
        static_cast<IRigidBody*>(pairHeader.actors[1]->userData);

    if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
    {
      actor0->OnCollisionEnter(rigidbody1);
      actor1->OnCollisionEnter(rigidbody0);
    }
    if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
    {
      actor0->OnCollisionExit(rigidbody1);
      actor1->OnCollisionExit(rigidbody0);
    }
  }
}

void PhyjixEventHandler::onTrigger(PxTriggerPair* pairs, PxU32 count) {}

void PhyjixEventHandler::onWake(PxActor** pActor, PxU32) {}

void PhyjixEventHandler::onSleep(PxActor**, PxU32) {}
