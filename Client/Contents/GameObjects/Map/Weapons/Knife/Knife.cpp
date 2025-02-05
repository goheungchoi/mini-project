#include "Knife.h"

#include "GameFramework/World/World.h"

Knife::Knife(World* world) : GameObject(world) {
  // Set a tag
  SetGameObjectTag("weapon");
}

void Knife::TurnOnCollision() {
	// TODO:

}

void Knife::TurnOffCollision() {
	// TODO:
}

void Knife::OnAwake()
{
  // Create a collider
  auto* rigidBody = CreateComponent<RigidbodyComponent>();
  rigidBody->Initialize({0, 0, 0}, Quaternion::Identity, {.1f, .1f, .1f},
                        ColliderShape::eCubeCollider, false, false,
                        world->_phyjixWorld);
  rigidBody->EnableDebugDraw();
}
