#pragma once
#include "IRigidBody.h"
#include <physx/PxPhysics.h>
class ICollisionEvent
{
public:
  virtual void OnCollisionEnter(IRigidBody* other) = 0;
  virtual void OnCollisionExit(IRigidBody* other) = 0;
  virtual void OnTrigger(IRigidBody* other) = 0;
  virtual void OnWake() = 0;
  virtual void OnSleep() = 0;
};