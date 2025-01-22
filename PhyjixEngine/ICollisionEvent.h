#pragma once
#include "physx/PxPhysicsAPI.h"
using namespace physx;
#include "IRigidBody.h"
class ICollisionEvent
{
public:
  virtual void OnCollisionEnter(IRigidBody* other) = 0;
  virtual void OnCollisionExit(IRigidBody* other) = 0;
  virtual void OnTrigger(IRigidBody* other) = 0;
  virtual void OnWake() = 0;
  virtual void OnSleep() = 0;
};