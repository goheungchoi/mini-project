#include "TestGameObject.h"
#include "GameFramework/World/World.h"

TestGameObject::TestGameObject(World* world) : GameObject(world)
{
  auto* billboard = CreateComponent<BillboardComponent>();
  world->_renderer->CreateBillboard(billboard->billboard);
  billboard->SetPosition(XMVectorSet(0.f, 5.f, 0.f, 1.f));
  billboard->SetScale(XMVectorSet(3.f, 3.f, 3.f, 1.f));
  //TextureHandle texHandle = LoadTexture()
  //billboard->SetTexture()
}

TestGameObject::~TestGameObject()
{
}