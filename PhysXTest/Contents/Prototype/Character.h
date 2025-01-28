#pragma once

#include "GameFramework/GameObject/GameObject.h"

class Character : public GameObject
{
protected:
  ModelHandle characterModelHandle;

public:

	Animation* idleAnimation;
  AnimationState* idleAnimationState;
  AnimatorComponent* animator;
  RigidbodyComponent* rigidbody;

	SkeletonHandle skeletonHandle;

  Character(World* world);

	~Character() { 
		
		
	}

  // Interaction
  virtual void OnBeginCursorOver() {};
  virtual void OnEndCursorOver() {};
  virtual void OnClicked() {};
  virtual void OnPressed() {};

  // Game loop events
  virtual void OnAwake() {}
  virtual void OnActivated() {}

  virtual void FixedUpdate(float fixedRate) {}
  virtual void PreUpdate(float dt) {}
  virtual void Update(float dt) {}
  virtual void PostUpdate(float dt) {}
  virtual void OnRender() {}
};
