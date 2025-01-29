#include "TestUI.h"
#include "GameFramework/UI/UIButton/UIButton.h"

TestUI::TestUI(World* world) : UIPanel(world)
{
  testBtn = CreateUI<UIButton>(L"TestButton");
  testBtn->SetPosition({200, 200});
  testBtn->AddOnClickHandler(	  [this]() { std::cout << "Click" << std::endl; });
  testBtn->AddOnHoveredHandler(	  [this]() { std::cout << "Hovered" << std::endl; });
  testBtn->AddOnPressedHandler(	  [this]() { std::cout << "Pressed" << std::endl; });
  testBtn->AddOnUnHoveredHandler( [this]() { std::cout << "UnHovered" << std::endl; });
}

TestUI::~TestUI() {}
