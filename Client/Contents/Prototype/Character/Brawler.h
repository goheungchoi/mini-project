#pragma once

#include "Character.h"

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
  virtual void OnBeginCursorOver() {};
  virtual void OnEndCursorOver() {};
  virtual void OnClicked() {};
  virtual void OnPressed() {};

  // Game loop events
  void OnAwake() override;
  virtual void OnActivated() {}

  virtual void FixedUpdate(float fixedRate) {}
  virtual void PreUpdate(float dt) {};
  void Update(float dt) override;
  virtual void PostUpdate(float dt) {}
  virtual void OnRender() {}
	

};
