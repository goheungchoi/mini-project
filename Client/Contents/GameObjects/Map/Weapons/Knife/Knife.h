#pragma once

#include "GameFramework/GameObject/GameObject.h"

class Knife : public GameObject
{

public:
  bool isCollsionOn{false};

  Knife(World* world);
  ~Knife() {}

	void TurnOnCollision();
  void TurnOffCollision();

  void Update(float dt) override;
};
