#pragma once

#include "GameFramework/GameObject/GameObject.h"

class Bullet : public GameObject
{
  float lifetime{5.f};

  float speed{1.4f};

	XMVECTOR direction{};

public:

	Bullet(World* world);
  ~Bullet() {}

	void SetDirection(XMVECTOR dir);

	void Update(float dt) override;
};
