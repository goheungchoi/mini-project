#pragma once

#include "Contents/GameObjects/Map/Characters/Character.h"

class Slasher : public Character
{
	using Super = Character;

protected:
  ModelHandle knifeModelHandle;

  GameObject* knife{nullptr};

	Animation* dead;
  AnimationState* deadState;

  Animation* idle;
  AnimationState* idleState;

  Animation* ready;
  AnimationState* readyState;

  Animation* action;
  AnimationState* actionState;

  GameObject* animationRoot;

	float slashSpeedPerSec{1.f};
  float slashElapsedTime{0.f};
  float slashNormalizedTime{0.f};

	bool temp{false};

	XMVECTOR currPos{};
  XMVECTOR targetPos{};

public:
  Slasher(World* world);
  ~Slasher();

	// Interaction

  // Game loop events
  void OnAwake() override;

  void Update(float dt) override;

};
