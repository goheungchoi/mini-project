#pragma once

#include "Contents/GameObjects/Map/Characters/Character.h"

class Civilian : public Character
{
	using Super = Character;
protected:
  Animation* dead;
  Animation* idle;
  Animation* surrender;

  AnimationState* deadState;
  AnimationState* idleState;
  AnimationState* surrenderState;

public:
  Civilian(World* world);
  ~Civilian();

  virtual void TriggerAction();

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
