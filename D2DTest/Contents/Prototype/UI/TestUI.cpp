#include "TestUI.h"
#include "GameFramework/UI/UIButton/UIButton.h"

TestUI::TestUI(World* world) : UIPanel(world)
{
  testBtn = CreateUI<UIButton>(L"TestButton");
  testBtn->SetPosition({200, 200});

  testBtn->AddOnClickHandler([this]() {
    _world->_renderer->AddText(L"클릭", {0, 0, 200, 200}, L"궁서");
  });

  testBtn->AddOnHoveredHandler([this]() {
    _world->_renderer->CreateSprite("Textures\\Test_CombatUI_1.png",
                                    {100, 100});
  });

  testBtn->AddOnPressedHandler([this]() {
    _world->_renderer->AddText(L"Pressed", {0, 0, 300, 100}, L"Agency FB");
  });

  testBtn->AddOnUnHoveredHandler(
      [this]() { std::cout << "UnHovered" << std::endl; });
}

TestUI::~TestUI() {}
