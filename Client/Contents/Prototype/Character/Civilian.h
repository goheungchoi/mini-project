#pragma once

#include "Character.h"

class Civilian : public Character
{
	using Super = Character;
protected:
  Animation* dead;
  Animation* idle;

  AnimationState* deadState;
  AnimationState* idleState;

public:
  Civilian(World* world);
  ~Civilian();

  //// Interaction
  //virtual void OnBeginCursorOver() {};
  //virtual void OnEndCursorOver() {};
  //virtual void OnClicked() {};
  //virtual void OnPressed() {};

  //// Game loop events
  //virtual void OnAwake() {}
  //virtual void OnActivated() {}

  //virtual void FixedUpdate(float fixedRate) {}
  //virtual void PreUpdate(float dt) {}
  //virtual void Update(float dt) {}
  //virtual void PostUpdate(float dt) {}
  //virtual void OnRender() {}
};
