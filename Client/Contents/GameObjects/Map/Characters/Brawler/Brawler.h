#pragma once

#include "Contents/GameObjects/Map/Characters/Character.h"

class Brawler : public Character
{
	using Super = Character;
protected:
  class Fist* fist{nullptr};

  Animation* dead;
  Animation* idle;
  Animation* action;

	AnimationState* deadState;
  AnimationState* idleState;
  AnimationState* actionState;

  std::pair<float, float> interval;

  bool bShout{false};
  bool bPunch{false};

public:
  Brawler(World* world);
  ~Brawler();

  // Interaction

  // Game loop events
  void OnAwake() override;

  void Update(float dt) override;
};
