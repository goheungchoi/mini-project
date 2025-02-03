#include "TestUI.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"

TestUI::TestUI(World* world) : UIPanel(world)
{
  testIMG = CreateUI<UIImage>(L"TestIMG");
  testIMG->SetSprite("Textures\\Test_QuestUI_1.png");
  testIMG->SetPosition({22, 20});

  testBtn = CreateUI<UIButton>(L"TestButton");
  testBtn->SetPosition({200, 200});

  testBtn->AddOnClickHandler([this]() {
    _world->_renderer->CreateText(
        L"사라진 아이들에 대한 단서 찾기",
        {0, 0, testIMG->GetSize().x, testIMG->GetSize().y}, L"궁서",
        {0, 0, 0, 1});
  });

  testBtn->AddOnHoveredHandler([this]() {
    _world->_renderer->CreateSprite("Textures\\Test_CombatUI_1.png",
                                    {1550, 850});
  });

  testBtn->AddOnPressedHandler([this]() {
    _world->_renderer->CreateText(
        L"All Enemies Defeated",
        {0, 0, testIMG->GetSize().x, testIMG->GetSize().y + 90}, L"Agency FB",
        {0, 0, 0, 1});
  });

  // testBtn->AddOnUnHoveredHandler([this]() {
  //   _world->_renderer->CreateSprite("Textures\\Test_QuestUI_1.png", {0, 0});
  // });
}

TestUI::~TestUI() {}

void TestUI::Render()
{
  // 작동안됨... 렌더 구조 잘못짜서 그런듯... TestGameObject와 비교 해볼 것
  // 주석 풀고 text 출력되는지 확인하면서 구조 짜기
  
  //_world->_renderer->DrawRectangle({0, 1, 1, 1}, {50, 50, 100, 100}, 2.0f);

  TextFormatInfo txtinfo;
  txtinfo._fontName = L"궁서";
  _world->_renderer->DrawTexts(L"TestUI 테스트", {300, 300, 1000, 1000},
                               {0, 1, 0, 1}, &txtinfo);
}
