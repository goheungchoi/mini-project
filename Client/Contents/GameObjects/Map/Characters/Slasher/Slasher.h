#pragma once

#include "Contents/GameObjects/Map/Characters/Character.h"

class Slasher : public Character
{
	using Super = Character;

protected:
  ModelHandle knifeModelHandle;

  class Knife* knife{nullptr};

	Animation* dead;
  AnimationState* deadState;

  Animation* idle;
  AnimationState* idleState;

  Animation* ready;
  AnimationState* readyState;

  Animation* action1;
  Animation* action2;
  AnimationState* actionState;

  Animation* currActionAnimation;
  std::pair<float, float> interval;

	float slashSpeedPerSec{1.f};
  float slashElapsedTime{0.f};
  float slashNormalizedTime{0.f};

	bool temp{false};

  bool isMoved{false};
	XMVECTOR currPos{};
  XMVECTOR targetPos{};


  bool bArm{false};
  bool bShout{false};
  bool bRushing{false};
  bool bStab{false};

public:
  Slasher(class World* world);
  ~Slasher();

  void TriggerAction() override;

  void TakeOverTargetCell();

  bool isCollsionOn{true};
  void TurnOnCollision();
  void TurnOffCollision();

	// Interaction
  void OnBeginOverlap(GameObject* other);

  // Game loop events
  void OnAwake() override;

  void Update(float dt) override;

};
