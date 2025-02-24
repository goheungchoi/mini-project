#pragma once

#include "Character.h"

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


	float slashSpeedPerSec{1.f};
  float slashElapsedTime{0.f};
  float slashNormalizedTime{0.f};

	bool temp{false};

	XMVECTOR currPos;
  XMVECTOR targetPos;

public:
  Slasher(World* world);
  ~Slasher() {}

	// Interaction
  virtual void OnBeginCursorOver() {};
  virtual void OnEndCursorOver() {};
  virtual void OnClicked() {};
  virtual void OnPressed() {};

  // Game loop events
  void OnAwake() override;
  virtual void OnActivated() {}

  virtual void FixedUpdate(float fixedRate) {}
  virtual void PreUpdate(float dt) {}
  void Update(float dt) override;
  virtual void PostUpdate(float dt) {}
  virtual void OnRender() {}

};
