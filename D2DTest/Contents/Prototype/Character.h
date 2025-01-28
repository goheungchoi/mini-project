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

	SkeletonHandle skeletonHandle;

  Character(World* world) : GameObject(world)
  {
    characterModelHandle = LoadModel("Models\\AnimTest\\AnimTest.glb");
    const auto& model = AccessModelData(characterModelHandle);

		skeletonHandle = model.skeleton;

		AnimationHandle idleAnimationHandle = *model.animations.begin();
    idleAnimation = new Animation(idleAnimationHandle, true);
    idleAnimationState = new AnimationState(idleAnimation);

		animator = CreateComponent<AnimatorComponent>();
    animator->BindSkeleton(skeletonHandle);
    animator->SetState(idleAnimationState);

		UnloadModel(characterModelHandle);

		auto* rigidBody = CreateComponent<RigidbodyComponent>();
    // rigidBody->Initialize({0, 0, 0}, {3, 5, 3}, ColliderShape::eCubeCollider, false, true, /* ? */);

  }

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
