#include "Bullet.h"

#include "GameFramework/World/World.h"

Bullet::Bullet(World* world) : GameObject(world)
{
	// Set a tag
  SetGameObjectTag("weapon");

  // Create a hand gun game object.

}

void Bullet::OnAwake()
{
  GameObject::OnAwake();

  
	  auto* rigidBody = CreateComponent<RigidbodyComponent>();
  auto v = transform->GetGlobalTranslation();

  rigidBody->Initialize({0, 0, 0}, Quaternion::Identity, {.1f, .1f, .1f},
                        ColliderShape::eCubeCollider, false, true,
                       GetWorld()->_phyjixWorld);
  rigidBody->SetCollisionEvent(nullptr, eCollisionEventType::eHover, [=]() {
    rigidBody->debugColor = Color(0, 1, 1, 1);
  });
  rigidBody->EnableDebugDraw();
  rigidBody->EnableSimulation();


}

void Bullet::SetDirection(XMVECTOR direction) {
  this->direction = direction;
}

void Bullet::Update(float dt) {

  GetComponent<RigidbodyComponent>()->debugColor = Color(1, 0, 1, 1);

  if (GetComponent<RigidbodyComponent>()->IsOverlapping())
  {
    GetComponent<RigidbodyComponent>()->debugColor = Color(0, 1, 1, 1);
  }



  lifetime -= dt;
  if (lifetime <= 0)
  {
    Destroy();
    return;
	}

  XMVECTOR velocity = dt * speed * direction;
  Translate(velocity);
}
