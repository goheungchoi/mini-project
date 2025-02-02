#pragma once

#include "Character.h"

class Gunman : public Character
{
	using Super = Character;
protected:
  ModelHandle handgunModelHandle;
  ModelHandle muzzleModelHandle;

	GameObject* handgun;

	GameObject* muzzle;
	
	Animation* dead;
  AnimationState* deadState;

	Animation* idle;
  AnimationState* idleState;

	Animation* ready1;
  AnimationState* ready1State;

	Animation* ready2;
  AnimationState* ready2State;

	Animation* fire;
  AnimationState* fireState;

public:

	Gunman(World* world);
  ~Gunman();

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
