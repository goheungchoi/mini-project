#include "Character.h"

#include "GameFramework/World/World.h"

Character::Character(World* world) : GameObject(world)
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

  /*auto* rigidBody = CreateComponent<RigidbodyComponent>();
  rigidBody->Initialize({0, 0, 0}, {3, 5, 3}, ColliderShape::eCubeCollider,
                        false, true, world->_phyjixWorld);*/
  // rigidBody->SetCollisionEvent()
}
