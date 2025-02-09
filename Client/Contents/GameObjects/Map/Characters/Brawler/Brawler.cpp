#include "Brawler.h"

#include "GameFramework/World/World.h"

#include "Contents/AnimationStates/IdleState.h"
#include "Contents/AnimationStates/BrawlerActionState.h"
#include "Contents/AnimationStates/DeadState.h"

#include "Contents/GameObjects/Map/Weapons/Fist/Fist.h"

#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"

Brawler::Brawler(World* world) : Character(world)
{
  type = kBrawler;
  range = 1;

  fist = world->CreateGameObject<Fist>();

	// NOTE: Test animation and state.
  dead = new Animation(deadAnimation, false);
  deadState = new DeadState(dead);

	idle = new Animation(idleAnimation, true);
  idleState = new IdleState(idle);

  action = new Animation(brawlerActionAnimation, false);
  actionState = new BrawlerActionState(action);

	idleState->AddAnimationStateDependency("next", actionState);
	idleState->AddAnimationStateDependency("dead", deadState);

	actionState->AddAnimationStateDependency("idle", idleState);
	actionState->AddAnimationStateDependency("dead", deadState);

	animator->SetState(idleState);

  interval = std::make_pair<float, float>(0.542f, 0.625f);
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

  GameObject* hand = FindChildGameObject("hand.r");
  if (hand)
  {
    hand->AddChildGameObject(fist);

    fist->SetInvisible();
  }
}

void Brawler::Update(float dt) {
  Super::Update(dt);

  if (!bShout && (0.12 <= action->GetCurrentAnimationTime()))
  {
    SoundManager::PlaySound(SoundList::Brawler_Ready);
    bShout = true;
  }

  // Turn on and off the knife collision.
  if (interval.first <= action->GetCurrentAnimationTime() &&
      action->GetCurrentAnimationTime() <= interval.second)
  {
    if (!bPunch)
    {
      SoundManager::PlaySound(SoundList::Brawler_Fire);
      bPunch = true;
    }

    if (!fist->isCollsionOn)
      fist->TurnOnCollision();
  }
  else
  {
    if (fist->isCollsionOn)
      fist->TurnOffCollision();
  }
}
