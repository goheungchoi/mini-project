#include "Bullet.h"

#include "GameFramework/World/World.h"
#include "Core/Camera/Camera.h"
Bullet::Bullet(World* world) : GameObject(world)
{
  // Set a tag
  SetGameObjectTag("weapon");
}

void Bullet::OnAwake()
{
  GameObject::OnAwake();

  auto* rigidBody = CreateComponent<RigidbodyComponent>();
  rigidBody->Initialize({0, 0, 0}, Quaternion::Identity, {.1f, .1f, .1f},
                        ColliderShape::eCubeCollider, false, true,
                        GetWorld()->_phyjixWorld);
  SetScaling(1.f);
  rigidBody->SetCollisionEvent(nullptr, eCollisionEventType::eHover, [=]() {
    rigidBody->debugColor = Color(0, 1, 1, 1);
  });
  rigidBody->EnableDebugDraw();
  rigidBody->EnableSimulation();

  // test
  auto* trail = CreateComponent<TrailComponent>();
  trail->SetDuration(0.5f);
  trail->SetWidth(0.15f);
  trail->SetDirection(direction);
  // can change color directX::simplemath::color
  //trail->SetColor({1.f, 0.f, 1.f, 1.f});
  // rotate to forward
  XMVECTOR forward = XMVectorSet(0, 0, -1.f, 0);
  XMVECTOR axis = XMVector3Normalize(XMVector3Cross(forward, direction));
  float angle =
      acosf(XMVectorGetX(XMVector3Dot(forward, XMVector3Normalize(direction))));
  if (angle != 0)
  {
    transform->SetRotationAroundAxis(axis, angle);
  }
}

void Bullet::SetDirection(XMVECTOR direction)
{
  this->direction = direction;
}

void Bullet::OnBeginOverlap(GameObject* other) {
  GameObject::OnBeginOverlap(other);

  if (other->GetGameObjectTag() == "Wall")
  {
    Destroy();
  }
}

void Bullet::Update(float dt)
{

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

  XMVECTOR velocity = dt *speed * direction;
  Translate(velocity);
}

void Bullet::PostUpdate(float dt)
{ 
  GetComponent<TrailComponent>()->Update(dt, transform->GetTranslation(),
                GetWorld()->mainCamera->GetPosition());
}
