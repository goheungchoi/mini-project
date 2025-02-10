#include "Civilian.h"

#include "Contents/AnimationStates/IdleState.h"
#include "Contents/AnimationStates/SurrenderState.h"
#include "Contents/AnimationStates/DeadState.h"

Civilian::Civilian(World* world) : Character(world) {
  type = kCivilian;
  range = 0;
  SetFaction(kNeutral);

	// Create animation states
  idle = new Animation(civilianIdleAnimation, true);
  idleState = new IdleState(idle);

	surrender = new Animation(civilianSurrenderAnimation, false);
  surrenderState = new SurrenderState(surrender);

	dead = new Animation(civilianDeadAnimation, false);
  deadState = new DeadState(dead);

	idleState->AddAnimationStateDependency("dead", deadState);
  idleState->AddAnimationStateDependency("next", surrenderState);
  surrenderState->AddAnimationStateDependency("dead", deadState);

	animator->SetState(idleState);
}

Civilian::~Civilian() {
  delete idle;
  delete idleState;
  delete surrender;
  delete surrenderState;
  delete dead;
  delete deadState;
}

void Civilian::TriggerAction() {
  isActionTriggered = true;

  HideOutline();
  HideDeathIndicator();

  animator->SetVariable<bool>("triggered", true, true);
}
