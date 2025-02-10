#pragma once

#include "GameFramework/GameObject/GameObject.h"
class Bullet : public GameObject
{
  float lifetime{3.f};

  float speed{1.4f * 8.f};

	XMVECTOR direction{};

public:
public:

	Bullet(World* world);
  ~Bullet() {}
	
	void SetDirection(XMVECTOR dir);

	void OnBeginOverlap(GameObject* other) override;

	void OnAwake() override;
	void Update(float dt) override;
  void PostUpdate(float dt) override;
};
