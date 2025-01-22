#pragma once
#include "ComponentBase.h"
//#include "../../PhyjixEngine/pch.h"
class RigidbodyComponent : public ComponentBase
{
  using Event = std::function<void(void)>;
public:
  void SetCollisionEvent();


private:
  //IRigidBody* _rigidbody = nullptr;
};