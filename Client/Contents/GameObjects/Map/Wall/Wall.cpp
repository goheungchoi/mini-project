#include "Wall.h"

#include "GameFramework/World/World.h"

Wall::Wall(World* world) : GameObject(world) 
{
  SetGameObjectTag("Wall");

  auto* rigidBody = CreateComponent<RigidbodyComponent>();
  rigidBody->Initialize({5.f, 0, 10.f}, Quaternion::Identity, {10.f, 10.f, 1.f},
                        ColliderShape::eCubeCollider, false, true,
                        GetWorld()->_phyjixWorld);
  rigidBody->SetCollisionEvent(nullptr, eCollisionEventType::eHover, [=]() {
    rigidBody->debugColor = Color(0, 1, 1, 1);
  });
  rigidBody->EnableDebugDraw();
  rigidBody->EnableSimulation();

}

