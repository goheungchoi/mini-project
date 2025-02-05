#pragma once

#include "GameFramework/GameObject/GameObject.h"

class Knife : public GameObject
{

public:
  Knife(World* world);
  ~Knife() {}

	void TurnOnCollision();
  void TurnOffCollision();

	void OnAwake() override;
};
