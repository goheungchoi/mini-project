#pragma once

#include "GameFramework/GameObject/GameObject.h"
class World;
class TestGameObject : public GameObject
{

public:
	
	TestGameObject(World* world);
  ~TestGameObject();
};


