#include "Fist.h"

#include "GameFramework/World/World.h"

Fist::Fist(World* world) : GameObject(world) {
  // Set a tag
  SetGameObjectTag("weapon");

  // Create a collider
  auto* rigidBody = CreateComponent<RigidbodyComponent>();
  rigidBody->Initialize({0.f, 0.1f, 0.f}, Quaternion::Identity,
                        {.08f, .2f, .05f}, ColliderShape::eCubeCollider, false,
                        true, GetWorld()->_phyjixWorld);
  rigidBody->DisableSimulation();
  
  rigidBody->EnableDebugDraw();
}

void Fist::TurnOnCollision() {
  auto* rigidBody = GetComponent<RigidbodyComponent>();
  if (rigidBody)
  {
    rigidBody->EnableSimulation();
    isCollsionOn = true;
  }
}

void Fist::TurnOffCollision() {
  auto* rigidBody = GetComponent<RigidbodyComponent>();
  if (rigidBody)
  {
    rigidBody->DisableSimulation();
    isCollsionOn = false;
  }
}

void Fist::Update(float dt) {
  GetComponent<RigidbodyComponent>()->debugColor = Color(1, 0, 1, 1);
  if (GetComponent<RigidbodyComponent>()->IsOverlapping())
  {
    GetComponent<RigidbodyComponent>()->debugColor = Color(0, 1, 1, 1);
  }
}
