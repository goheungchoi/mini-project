#include "TestGameObject.h"
#include "GameFramework/World/World.h"

void TestGameObject::OnRender() 
{
  GetWorld()->_renderer->TextDraw(L"ÇÑ±ÛTest1234", {0, 0, 1920, 150}, L"Agency FB");
}