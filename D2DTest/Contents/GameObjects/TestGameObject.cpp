#include "TestGameObject.h"
#include "GameFramework/World/World.h"

#include "GameFramework/Level/Level.h"

TestGameObject::TestGameObject(World* world)
    : GameObject(world)
{
  //world->_renderer->CreateSprite("Models\\Sponza\\2775690330959970771.jpg");
  //world->_renderer->CreateSprite("Textures/BakerSpecularBRDF_LUT.dds");
  //world->_renderer->CreateSprite("Textures\\Test_CombatUI_1.png", {100,100});

  //GetWorld()->_renderer->CreateText(L"한글Test1234", {0, 0, 1920, 150}, L"궁서");
  //GetWorld()->_renderer->CreateText(L"asdfqwer", {0, 0, 300, 150}, L"Agency FB");
}
void TestGameObject::OnRender() 
{
  GetWorld()->_renderer->DrawRectangle({1, 1, 1, 1}, {0, 0, 200, 200}, 1.0f);

  //TextFormatInfo txtinfo;
  //txtinfo._fontName = L"궁서";
  //GetWorld()->_renderer->DrawTexts(L"텍스트Test1234", {300, 500, 1000, 1000},
  //                                 {0, 1, 0, 1}, &txtinfo);
}