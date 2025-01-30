#include "Brawler.h"

#include "GameFramework/World/World.h"

#include "Contents/AnimationStates/IdleState.h"
#include "Contents/AnimationStates/BrawlerActionState.h"
#include "Contents/AnimationStates/DeadState.h"

Brawler::Brawler(World* world) : Character(world)
{
  type = kBrawler;
  range = 1;

	// NOTE: Test animation and state.
  dead = new Animation(*characterModelData->animations.begin(), false);
  deadState = new DeadState(dead);

	idle = new Animation(*characterModelData->animations.begin(), true);
  idleState = new IdleState(idle);

  action = new Animation(*characterModelData->animations.begin(), false);
  actionState = new BrawlerActionState(action);

	idleState->AddAnimationStateDependency("next", actionState);
	idleState->AddAnimationStateDependency("dead", deadState);

	actionState->AddAnimationStateDependency("idle", idleState);
	actionState->AddAnimationStateDependency("dead", deadState);

	animator->SetState(idleState);
}

Brawler::~Brawler() {
  delete actionState;
  delete action;
  delete idleState;
  delete idle;
  delete deadState;
  delete dead;
}

void Brawler::OnAwake() {
  Super::OnAwake();

}

void Brawler::Update(float dt) {
  Super::Update(dt);


}
