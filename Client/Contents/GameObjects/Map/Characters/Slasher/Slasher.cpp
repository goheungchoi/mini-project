#include "Slasher.h"

#include "GameFramework/World/World.h"

#include "Contents/AnimationStates/IdleState.h"
#include "Contents/AnimationStates/SlashReadyState.h"
#include "Contents/AnimationStates/SlashActionState.h"
#include "Contents/AnimationStates/DeadState.h"

#include "Contents/GameObjects/Map/Weapons/Knife/Knife.h"

Slasher::Slasher(World* world) : Character(world) {
  type = kSlasher;
  range = 2;

	// Create a knife
  /*handgunModelHandle = LoadModel("Models\\HandGun\\HandGun.glb");
  handgun = world->CreateGameObjectFromModel(handgunModelHandle);
  UnloadModel(handgunModelHandle);*/

  knifeModelHandle = LoadModel("Models\\Knife\\Knife.glb");
  knife = world->CreateGameObjectFromModel<Knife>(knifeModelHandle);

	// Create animation states
  dead = new Animation(deadAnimation, false);
  deadState = new DeadState(dead);

  idle = new Animation(idleAnimation, true);
  idleState = new IdleState(idle);

  ready = new Animation(slashReadyAnimation, false);
  readyState = new SlashReadyState(ready);

  action1 = new Animation(slashAction1Animation, false);

  action2 = new Animation(slashAction2Animation, false);
  actionState = new SlashActionState(action2);

  currActionAnimation = action2;
  interval = std::make_pair<float, float>(0.3125f, 0.375f);

  animator->DeclareVariable<bool>("arm", false);
	animator->DeclareVariable<bool>("fire", false);

	// Set state dependencies
  idleState->AddAnimationStateDependency("next", readyState);
  idleState->AddAnimationStateDependency("dead", deadState);

  readyState->AddAnimationStateDependency("next", actionState);
  readyState->AddAnimationStateDependency("dead", deadState);

  actionState->AddAnimationStateDependency("dead", deadState);
  actionState->AddAnimationStateDependency("idle", idleState);

  animator->SetState(idleState);
}

Slasher::~Slasher() {
  delete dead;
  delete deadState;
  delete idle;
  delete idleState;
  delete ready;
  delete readyState;
  delete action1;
  delete action2;
  delete actionState;
}

void Slasher::TriggerAction() {
  Super::TriggerAction();

  if (isTargetInRange)
  {
    if (distanceToTarget < range)
    {
      actionState->SetAnimation(action1);
      currActionAnimation = action1;
      interval = std::make_pair<float, float>(.208f, .271f);
    }
  }
}

void Slasher::TakeOverTargetCell() {
  if (isTargetInRange)
  {
    auto [w_offset, h_offset] = GetGridFrontDirection();
    auto [target_w, target_h] =
        std::make_pair<int, int>(grid_w + distanceToTarget * w_offset,
                                 grid_h + distanceToTarget * h_offset);

    SetGridLocation(target_w, target_h);
  }
}

void Slasher::TurnOnCollision() {
  auto* rigidBody = GetComponent<RigidbodyComponent>();
  if (rigidBody)
  {
    rigidBody->EnableSimulation();
    isCollsionOn = true;
  }
}

void Slasher::TurnOffCollision() {
  auto* rigidBody = GetComponent<RigidbodyComponent>();
  if (rigidBody)
  {
    rigidBody->DisableSimulation();
    isCollsionOn = false;
  }
}

void Slasher::OnBeginOverlap(GameObject* other) {
  Super::OnBeginOverlap(other);

  if (!isDead)
  {
    // If slashers are colliding with each other,
    // both are dead.
    if (other->typeIndex == this->typeIndex)
    {
      auto* otherSlasher = (Slasher*)other;
      otherSlasher->Die();
      this->Die();
    }
  }
}

void Slasher::OnAwake() {
  Super::OnAwake();

  GameObject* hand = FindChildGameObject("hand.r");
  if (hand)
  {
    hand->AddChildGameObject(knife);
    knife->RotateAroundXAxis(XM_PI);
    knife->RotateAroundYAxis(-XM_PIDIV2);
    knife->RotateAroundZAxis(-XM_PIDIV2);
    knife->RotateAroundYAxis(XM_PIDIV2);
    knife->RotateAroundZAxis(XM_PIDIV2);
    knife->Translate(-0.2f, 0.1f, 0.04f);

    knife->SetInvisible();
  }
}

void Slasher::Update(float dt) {
  Super::Update(dt);

  if (animator->GetVariable<bool>("arm"))
  {
    knife->SetVisible();
    animator->SetVariable<bool>("arm", false);
  }

  if (animator->GetVariable<bool>("fire"))
  {
    TurnOffCollision();
    animator->SetVariable<bool>("fire", false);
  }

  // Turn on and off the knife collision.
  if (interval.first <= currActionAnimation->GetCurrentAnimationTime() &&
      currActionAnimation->GetCurrentAnimationTime() <= interval.second)
  {
    if (!knife->isCollsionOn)
      knife->TurnOnCollision();
  }
  else
  {
    if (knife->isCollsionOn)
      knife->TurnOffCollision();
  }

  if (!isMoved && currActionAnimation->GetCurrentAnimationTime() >= 0.75)
  {
    TurnOnCollision();
    TakeOverTargetCell();
    isMoved = true;
  }

	if (animator->GetVariable<bool>("done"))
  {
    animator->SetVariable<bool>("done", false);
  }
}
