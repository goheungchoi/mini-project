#include "TestUI.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"

TestUI::TestUI(World* world) : UIPanel(world)
{
  testIMG = CreateUI<UIImage>(L"TestIMG");
  testIMG->SetSprite("Textures\\Test_QuestUI_1.png");
  //testIMG->SetPosition({0, 300}); // 지금 이거 동작 안됨.

  testBtn = CreateUI<UIButton>(L"TestButton");
  testBtn->SetPosition({200, 200});
  
  testBtn->AddOnClickHandler([this]() {
    _world->_renderer->CreateText(L"클릭", {0, 0, 200, 200}, L"궁서");
  });

  testBtn->AddOnHoveredHandler([this]() {
    _world->_renderer->CreateSprite("Textures\\Test_CombatUI_1.png",
                                    {100, 100});
  });

  testBtn->AddOnPressedHandler([this]() {
    _world->_renderer->CreateText(L"Pressed", {0, 0, 300, 100}, L"Agency FB");
  });

  //testBtn->AddOnUnHoveredHandler([this]() {
  //  _world->_renderer->CreateSprite("Textures\\Test_QuestUI_1.png", {0, 0});
  //});

}

TestUI::~TestUI() {}

void TestUI::Render() 
{
    // 작동안됨... 렌더 구조 잘못짜서 그런듯... TestGameObject와 비교 해볼 것
  //_world->_renderer->DrawRectangle({0, 1, 1, 1}, {50, 50, 100, 100}, 2.0f);
}
