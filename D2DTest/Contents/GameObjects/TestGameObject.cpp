#include "TestGameObject.h"
#include "GameFramework/World/World.h"

void TestGameObject::OnRender() 
{
  GetWorld()->_renderer->TextDraw(L"한글Test1234", {0, 0, 1920, 150}, L"궁서");
}