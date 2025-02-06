#pragma once

#include "GameFramework/GameObject/GameObject.h"

class Obstacle : public GameObject
{
public:

	Obstacle(class World* world) : GameObject(world)
  {
    SetGameObjectTag("Obstacle");
	}
  ~Obstacle();

};
