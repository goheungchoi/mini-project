#pragma once

#include "GameFramework/GameObject/GameObject.h"

class Fist : public GameObject
{

public:
  Fist(World* world);
  ~Fist() {}

  void TurnOnCollision();
  void TurnOffCollision();

  void OnAwake() override;
};
