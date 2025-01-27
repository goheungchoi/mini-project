#pragma once

#include "GameFramework/GameObject/GameObject.h"
#include "GameFramework/Components/RigidbodyComponent.h"
class TestGameObject : public GameObject
{

public:
  RigidbodyComponent* _rigidbody;
  TestGameObject(World* world) : GameObject(world)
  {
    _rigidbody = CreateComponent<RigidbodyComponent>();
    

  }

};
