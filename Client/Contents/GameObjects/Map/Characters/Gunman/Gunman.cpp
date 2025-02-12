#include "Gunman.h"

#include "GameFramework/World/World.h"

#include "Contents/AnimationStates/IdleState.h"
#include "Contents/AnimationStates/GunFireReady1State.h"
#include "Contents/AnimationStates/GunFireReady2State.h"
#include "Contents/AnimationStates/GunFireActionState.h"
#include "Contents/AnimationStates/DeadState.h"

#include "Contents/GameObjects/Map/Weapons/Bullet/Bullet.h"

#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"

Gunman::Gunman(World* world) : Character(world) {
  type = kGunman;
  range = 5;

	// Create a hand gun game object.
  handgunModelHandle = LoadModel("Models\\Pistol\\Pistol.glb");
	handgun = world->CreateGameObjectFromModel(handgunModelHandle);

  // Create a muzzle game object.
	muzzleModelHandle = LoadModel("Models\\Bullet\\Bullet.glb");
	muzzle = world->CreateGameObjectFromModel<GameObject>(muzzleModelHandle);
  muzzle->SetTranslation(0.f, .01f, -.2f);
  muzzle->SetInvisible();

	handgun->AddChildGameObject(muzzle);
  
	// Create animation states
  dead = new Animation(deadAnimation, false);
  deadState = new DeadState(dead);

  idle = new Animation(idleAnimation, true);
  idleState = new IdleState(idle);

  ready1 = new Animation(gunReady1Animation, false);
  ready1State = new GunFireReady1State(ready1);

  ready2 = new Animation(gunReady2Animation, false);
  ready2State = new GunFireReady2State(ready2);

  fire = new Animation(gunFireAnimation, false);
  fireState = new GunFireActionState(fire);

  animator->DeclareVariable<bool>("arm", false);
	animator->DeclareVariable<bool>("fire", false);

	// Set state dependencies
  idleState->AddAnimationStateDependency("next", ready1State);
  idleState->AddAnimationStateDependency("dead", deadState);

  ready1State->AddAnimationStateDependency("next", ready2State);
  ready1State->AddAnimationStateDependency("dead", deadState);

	ready2State->AddAnimationStateDependency("next", fireState);
  ready2State->AddAnimationStateDependency("dead", deadState);

	fireState->AddAnimationStateDependency("dead", deadState);

	animator->SetState(idleState);
}

Gunman::~Gunman() {
  delete dead;
  delete deadState;
  delete idle;
  delete idleState;
  delete ready1;
  delete ready1State;
  delete ready2;
  delete ready2State;
  delete fire;
  delete fireState;

	UnloadModel(muzzleModelHandle);
  UnloadModel(handgunModelHandle);
}

void Gunman::OnAwake() {
  Super::OnAwake();

	GameObject* hand = FindChildGameObject("hand.r");
  if (hand)
  {
		hand->AddChildGameObject(handgun);
    handgun->RotateAroundXAxis(XM_PI);
    handgun->RotateAroundYAxis(-XM_PIDIV2);
    handgun->RotateAroundZAxis(-XM_PIDIV2);
    handgun->Translate(-0.06f, 0.18f, 0.04f);

    handgun->SetInvisible();
	}
}

void Gunman::Update(float dt) {
  Super::Update(dt);

  if (animator->GetVariable<bool>("arm") && !bPullOutGun)
  {
    SoundManager::PlaySound(SoundList::Gunman_Ready1);
    handgun->SetVisible();
    bPullOutGun = true;
  }

  if (animator->GetVariable<bool>("arm") && !bRackSlide &&
      (0.65f <= ready2->GetCurrentAnimationTime()))
  {
    SoundManager::PlaySound(SoundList::Gunman_Ready2);
    bRackSlide = true;
  }

  if (animator->GetVariable<bool>("fire") && !bHammerSpur &&
      (0.54f <= fire->GetCurrentAnimationTime()))
  {
    SoundManager::PlaySound(SoundList::Gunman_Ready3);
    bHammerSpur = true;
  }

	if (animator->GetVariable<bool>("fire") && !bFireBullet &&
      (0.65f <= fire->GetCurrentAnimationTime()))
  {
    Bullet* bullet =
        world->CreateGameObjectFromModel<Bullet>(muzzleModelHandle);
    bullet->SetTranslation(muzzle->transform->GetGlobalTranslation());
    bullet->transform->globalTransform =
        muzzle->transform->GetGlobalTransform();
    bullet->SetDirection(-this->GetGlobalFront());
    bullet->SetScaling(10.f, 10.f, 10.f);

    SoundManager::PlaySound(GetAny(SoundList::Gunman_Fire));

		bFireBullet = true;
	}
}
