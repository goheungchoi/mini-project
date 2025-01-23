#pragma once

#include "IRigidBody.h"
class ICollisionEvent
{
public:
  virtual ~ICollisionEvent() = default;
  virtual void OnCollisionEnter(IRigidBody* other) = 0;
  virtual void OnCollisionExit(IRigidBody* other) = 0;
  virtual void OnTrigger(IRigidBody* other) = 0;
  virtual void OnWake() = 0;
  virtual void OnSleep() = 0;
  virtual void OnHover() = 0;
  virtual void OnLeftClick() = 0;
  virtual void OnRightClick() = 0;
};