#include "TestGameObject.h"
#include "GameFramework/World/World.h"

#include "GameFramework/Level/Level.h"

TestGameObject::TestGameObject(World* world)
    : GameObject(world)
{
  //world->_renderer->CreateSprite("Models\\Sponza\\2775690330959970771.jpg");
  //world->_renderer->CreateSprite("Textures/BakerSpecularBRDF_LUT.dds");
  world->_renderer->CreateSprite("Textures\\Test_CombatUI_1.png");

}
void TestGameObject::OnRender() 
{
  GetWorld()->_renderer->AddText(L"한글Test1234", {0, 0, 1920, 150}, L"궁서");
  GetWorld()->_renderer->AddText(L"asdfqwer", {0, 0, 300, 150}, L"Agency FB");
}