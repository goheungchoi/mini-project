#include "Knife.h"

#include "GameFramework/World/World.h"

Knife::Knife(World* world) : GameObject(world) {
  // Set a tag
  SetGameObjectTag("weapon");

  // Create a collider
  auto* rigidBody = CreateComponent<RigidbodyComponent>();
  rigidBody->Initialize({0.f, 0.0f, -.18f}, Quaternion::Identity, {.02f, .02f, .1f},
                        ColliderShape::eCubeCollider, false, true,
                        world->_phyjixWorld);
  rigidBody->DisableSimulation();
  rigidBody->EnableDebugDraw();
}

void Knife::TurnOnCollision() {
  auto* rigidBody = GetComponent<RigidbodyComponent>();
  if (rigidBody)
  {
    rigidBody->EnableSimulation();
    isCollsionOn = true;
  }
}

void Knife::TurnOffCollision() {
  auto* rigidBody = GetComponent<RigidbodyComponent>();
  if (rigidBody)
  {
    rigidBody->DisableSimulation();
    isCollsionOn = false;
  }
}

void Knife::Update(float dt) {
  GetComponent<RigidbodyComponent>()->debugColor = Color(1, 0, 1, 1);
  if (GetComponent<RigidbodyComponent>()->IsOverlapping())
  {
    GetComponent<RigidbodyComponent>()->debugColor = Color(0, 1, 1, 1);
  }
}
