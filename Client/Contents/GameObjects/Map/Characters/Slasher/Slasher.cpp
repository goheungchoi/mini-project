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

  knifeModelHandle = LoadModel("Models\\Knife\\Knife.glb");
  knife = world->CreateGameObjectFromModel(knifeModelHandle);

	// Create animation states
  dead = new Animation(deadAnimation, false);
  deadState = new DeadState(dead);

  idle = new Animation(idleAnimation, true);
  idleState = new IdleState(idle);

  ready = new Animation(slashReadyAnimation, false);
  readyState = new SlashReadyState(ready);

  action = new Animation(slashActionAnimation, false);
  actionState = new SlashActionState(action);

  animator->DeclareVariable<bool>("arm", false);
	animator->DeclareVariable<bool>("fire", false);
  animator->DeclareVariable<XMVECTOR>("currTranslate");

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
  delete action;
  delete actionState;
}

void Slasher::OnAwake() {
  Super::OnAwake();
  /*GameObject* hand = FindChildGameObject("hand.r");
  hand->AddChildGameObject(handgun);*/

  animationRoot = FindChildGameObject("c_pos");

  GameObject* hand = FindChildGameObject("hand.r");
  if (hand)
  {
    hand->AddChildGameObject(knife);
    knife->RotateAroundXAxis(XM_PI);
    knife->RotateAroundYAxis(-XM_PIDIV2);
    knife->RotateAroundZAxis(-XM_PIDIV2);
    knife->RotateAroundYAxis(XM_PIDIV2);
    knife->Translate(-0.06f, 0.32f, 0.04f);

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

	if (animator->GetVariable<bool>("done"))
  {
    // TODO: slash!
    /*if (!temp)
    {
      currPos = transform->GetTranslation();
      targetPos =
          1.4f * distanceToTarget * -transform->GetLocalFront() + currPos;
			temp = true;
    }
    else
    {
      slashElapsedTime += dt;
      float normalizedTime = slashElapsedTime / (float)distanceToTarget;
      float t = MathUtil::bezier::ease_in(normalizedTime);

      SetTranslation(t * targetPos + (1 - t) * currPos);

      if (normalizedTime >= 1.f)
        animator->SetVariable<bool>("done", false);
		}*/

    SetTranslation(animator->GetVariable<XMVECTOR>("currTranslate"));
    animator->SetVariable<bool>("done", false);
  }
}
