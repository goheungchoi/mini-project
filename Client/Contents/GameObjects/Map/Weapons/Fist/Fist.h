#pragma once

#include "GameFramework/GameObject/GameObject.h"

class Fist : public GameObject
{

public:
  bool isCollsionOn{false};

  Fist(World* world);
  ~Fist() {}

  void TurnOnCollision();
  void TurnOffCollision();

  void Update(float dt) override;
};
