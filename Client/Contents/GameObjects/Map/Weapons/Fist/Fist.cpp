#include "Fist.h"

#include "GameFramework/World/World.h"

Fist::Fist(World* world) : GameObject(world) {
  // Set a tag
  SetGameObjectTag("weapon");
}

void Fist::TurnOnCollision() {}

void Fist::TurnOffCollision() {}

void Fist::OnAwake() {
  // Create a collider
  auto* rigidBody = CreateComponent<RigidbodyComponent>();
  rigidBody->Initialize({0, 0, 0}, Quaternion::Identity, {.1f, .1f, .1f},
                        ColliderShape::eCubeCollider, false, false,
                        world->_phyjixWorld);
  rigidBody->EnableDebugDraw();
}
