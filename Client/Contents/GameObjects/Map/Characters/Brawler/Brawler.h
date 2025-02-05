#pragma once

#include "Contents/GameObjects/Map/Characters/Character.h"

class Brawler : public Character
{
	using Super = Character;
protected:
  Animation* dead;
  Animation* idle;
  Animation* action;

	AnimationState* deadState;
  AnimationState* idleState;
  AnimationState* actionState;

public:
  Brawler(World* world);
  ~Brawler();

  // Interaction

  // Game loop events
  void OnAwake() override;

  void Update(float dt) override;
};
