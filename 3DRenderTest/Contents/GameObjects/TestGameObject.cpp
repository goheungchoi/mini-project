#include "TestGameObject.h"
#include "GameFramework/World/World.h"

TestGameObject::TestGameObject(World* world) : GameObject(world)
{
  auto* billboard = CreateComponent<BillboardComponent>();
  world->_renderer->CreateBillboard(billboard->billboard);
}

TestGameObject::~TestGameObject()
{
}