#include "Civilian.h"

#include "Contents/AnimationStates/IdleState.h"
#include "Contents/AnimationStates/BrawlerActionState.h"
#include "Contents/AnimationStates/DeadState.h"

Civilian::Civilian(World* world) : Character(world) {
  faction = kNeutral;
  type = kCivilian;
  range = 0;


	// Create animation states
	idle = new Animation(*characterModelData->animations.begin(), true);
	idleState = new AnimationState(idle);

	dead = new Animation(*characterModelData->animations.begin(), false);
  deadState = new AnimationState(dead);

	idleState->AddAnimationStateDependency("dead", deadState);

	animator->SetState(idleState);
}

Civilian::~Civilian() {}
