#include "Bullet.h"

#include "GameFramework/World/World.h"

Bullet::Bullet(World* world) : GameObject(world)
{
	// Set a tag
  SetGameObjectTag("weapon");

  // Create a hand gun game object.
  auto* rigidBody = CreateComponent<RigidbodyComponent>();
  rigidBody->Initialize({0, 0, 0}, Quaternion::Identity, {.1f, .1f, .1f},
                            ColliderShape::eCubeCollider, false, false,
                            world->_phyjixWorld);
  rigidBody->EnableDebugDraw();
}

void Bullet::SetDirection(XMVECTOR direction) {
  this->direction = direction;
}

void Bullet::Update(float dt) {
  lifetime -= dt;
  if (lifetime <= 0)
  {
    Destroy();
    return;
	}

  XMVECTOR velocity = dt * speed * direction;
  Translate(velocity);
}
