#include "Slasher.h"

#include "GameFramework/World/World.h"

#include "Contents/AnimationStates/IdleState.h"
#include "Contents/AnimationStates/SlashReadyState.h"
#include "Contents/AnimationStates/SlashActionState.h"
#include "Contents/AnimationStates/DeadState.h"

Slasher::Slasher(World* world) : Character(world) {
  type = kSlasher;
  range = 2;

	// Create a knife
  /*handgunModelHandle = LoadModel("Models\\HandGun\\HandGun.glb");
  handgun = world->CreateGameObjectFromModel(handgunModelHandle);
  UnloadModel(handgunModelHandle);*/


	// Animator one more variable to move slasher
  animator->DeclareVariable<bool>("action");

	// Create animation states
  dead = new Animation(*characterModelData->animations.begin(), false);
  deadState = new AnimationState(dead);

  idle = new Animation(*characterModelData->animations.begin(), true);
  idleState = new AnimationState(idle);

  ready = new Animation(*characterModelData->animations.begin(), false);
  readyState = new AnimationState(ready);

  action = new Animation(*characterModelData->animations.begin(), false);
  actionState = new AnimationState(action);

	// Set state dependencies
  idleState->AddAnimationStateDependency("next", readyState);
  idleState->AddAnimationStateDependency("dead", deadState);

  readyState->AddAnimationStateDependency("next", actionState);
  readyState->AddAnimationStateDependency("dead", deadState);

  actionState->AddAnimationStateDependency("dead", deadState);
  actionState->AddAnimationStateDependency("idle", idleState);

  animator->SetState(idleState);
}

void Slasher::OnAwake() {
  Super::OnAwake();
  /*GameObject* hand = FindChildGameObject("hand.r");
  hand->AddChildGameObject(handgun);*/
}

void Slasher::Update(float dt) {
  Super::Update(dt);

}
