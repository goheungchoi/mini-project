#include "Gunman.h"

#include "GameFramework/World/World.h"

#include "Contents/AnimationStates/IdleState.h"
#include "Contents/AnimationStates/GunFireReady1State.h"
#include "Contents/AnimationStates/GunFireReady2State.h"
#include "Contents/AnimationStates/GunFireActionState.h"
#include "Contents/AnimationStates/DeadState.h"

#include "Contents/Prototype/Bullet/Bullet.h"

Gunman::Gunman(World* world) : Character(world) {
  type = kGunman;
  range = 5;

	// Create a hand gun game object.
  handgunModelHandle = LoadModel("Models\\HandGun\\HandGun.glb");
	handgun = world->CreateGameObjectFromModel(handgunModelHandle);

	muzzleModelHandle = LoadModel("Models\\Bullet\\Bullet.glb");
	muzzle = world->CreateGameObjectFromModel<GameObject>(muzzleModelHandle);

	handgun->AddChildGameObject(muzzle);
  
	// Create a muzzle game object.


	// Create animation states
  dead = new Animation(*characterModelData->animations.begin(), false);
  deadState = new DeadState(dead);

  idle = new Animation(*characterModelData->animations.begin(), true);
  idleState = new IdleState(idle);

  ready1 = new Animation(*characterModelData->animations.begin(), false);
  ready1State = new GunFireReady1State(ready1);

  ready2 = new Animation(*characterModelData->animations.begin(), false);
  ready2State = new GunFireReady2State(ready2);

  fire = new Animation(*characterModelData->animations.begin(), false);
  fireState = new GunFireActionState(fire);

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
  UnloadModel(handgunModelHandle);
}

void Gunman::OnAwake() {
  Super::OnAwake();

	GameObject* hand = FindChildGameObject("hand.r");
  if (hand)
  {
		hand->AddChildGameObject(handgun);
	}


}

void Gunman::Update(float dt) {
  Super::Update(dt);

	if (animator->GetVariable<bool>("fire"))
  {
    Bullet* bullet =
        world->CreateGameObjectFromModel<Bullet>(muzzleModelHandle);
    bullet->SetTranslation(muzzle->transform->GetGlobalTranslation());
    bullet->SetDirection(-this->GetGlobalFront());
    bullet->SetScaling(10.f, 10.f, 10.f);

		animator->SetVariable<bool>("fire", false);
	}
  auto rb = GetComponent<RigidbodyComponent>();
  //rb->ClearTorque();
  //if (INPUT.IsKeyPress(Key::O)) rb->Rotate({-10, 0, 0});
  //if (INPUT.IsKeyPress(Key::P)) rb->Rotate({0, 0, 10});
  //if (INPUT.IsKeyPress(Key::Left)) rb->Translate({-10, 0, 0});
  //if (INPUT.IsKeyPress(Key::Right)) rb->Translate({10, 0, 0});
  //if (INPUT.IsKeyPress(Key::Up)) rb->Translate({0, 0, 10});
  //if (INPUT.IsKeyPress(Key::Down)) rb->Translate({0, 0, -10});
  Vector3 pos ={0,0,0};
  //Quaternion rot = GetComponent<TransformComponent>()->GetGlobalQuaternion();
  float movespeed = 5.0f;
  if (INPUT.IsKeyPress(Key::Left))
    pos += Vector3(-dt * movespeed, 0, 0);
  if (INPUT.IsKeyPress(Key::Right))
    pos += Vector3(dt * movespeed, 0, 0);
  if (INPUT.IsKeyPress(Key::Up))
    pos += Vector3(0, 0, dt * movespeed);
  if (INPUT.IsKeyPress(Key::Down))
    pos += Vector3(0, 0, -dt * movespeed);

  //rb->SetKinematicTransform(pos, rot);
  transform->Translate(pos);
}
