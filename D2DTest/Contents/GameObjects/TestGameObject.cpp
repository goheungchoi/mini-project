#include "TestGameObject.h"
#include "GameFramework/World/World.h"

#include "GameFramework/Level/Level.h"

TestGameObject::TestGameObject(World* world)
    : GameObject(world)
{
  world->_renderer->CreateSprite("Textures/BakerSpecularBRDF_LUT.dds");
}
void TestGameObject::OnRender() 
{
  GetWorld()->_renderer->TextDraw(L"ÇÑ±ÛTest1234", {0, 0, 1920, 150}, L"Agency FB");
}