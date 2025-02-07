#include "DialogUI.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"
#include "GameFramework/UI/UIText/UIText.h"
#include "Shared/Config/Config.h"

DialogUI::DialogUI(class World* world) : UIPanel(world)
{
  {
    _PhotoPanel = CreateUI<UIImage>(L"PhotoPanel");
    _PhotoPanel->SetSprite("Textures\\Picture\\PhotoPanel.png");
    _PhotoPanel->SetCenterPos({960, 540});
  }
  {
    fs::path p = ns::kProjectDir;
    std::string rootpath = p.string();
    _Eliza = CreateUI<UIAnim>(L"Eliza");
    std::string gunfire =
        "\\Library\\CSV\\Eliza_Animation\\Eliza_Initiative_Gunfire.csv";
    std::string noble = "\\Library\\CSV\\Eliza_Animation\\Eliza_Noble.csv";
    std::string d30 =
        "\\Library\\CSV\\Eliza_Animation\\Eliza_Resistance_Despair30.csv";
    std::string d70 =
        "\\Library\\CSV\\Eliza_Animation\\Eliza_Resistance_Despair70.csv";
    std::string joy =
        "\\Library\\CSV\\Eliza_Animation\\Eliza_Resistance_Joy.csv";
    std::string sad =
        "\\Library\\CSV\\Eliza_Animation\\Eliza_Resistance_Sad.csv";
    _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Noble.png", rootpath + noble);
    _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Initiative_Gunfire.png",
                           rootpath + gunfire);
    _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Resistance_Despair30.png",
                           rootpath + d30);
    _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Resistance_Despair70.png",
                           rootpath + d70);
    _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Resistance_Joy.png",
                           rootpath + joy);
    _Eliza->LoadAnimSprite("2D\\Animation\\Eliza_Resistance_Sad.png",
                           rootpath + sad);
    _Eliza->SetCenterPos("Eliza_Noble", {100, 70});
    _Eliza->SetScale("Eliza_Noble", {.88f, .88f});
    _Eliza->SetCenterPos("Eliza_Resistance_Despair30", {35, 155});
    _Eliza->SetScale("Eliza_Resistance_Despair30", {.88f, .88f});
    _Eliza->SetCenterPos("Eliza_Resistance_Despair70", {28, 161});
    _Eliza->SetScale("Eliza_Resistance_Despair70", {.88f, .88f});
    _Eliza->SetCenterPos("Eliza_Resistance_Joy", {35, 152.5f});
    _Eliza->SetScale("Eliza_Resistance_Joy", {.88f, .88f});
    _Eliza->SetCenterPos("Eliza_Resistance_Sad", {35, 156});
    _Eliza->SetScale("Eliza_Resistance_Sad", {.88f, .88f});
    _Eliza->SetCurrentAnimSprite("Eliza_Noble");
  }

  {
    _dialogTextBox = CreateUI<UIImage>(L"DialogTextBox");
    _dialogTextBox->SetSprite("2D\\UI\\UI_Textbox_01.png");
    _dialogTextBox->SetCenterPos({960, 900});
    _dialogTextBox->SetOpacity(0.f);
    _actionList.push_back([=]() { _dialogTextBox->FadeIn(1.0f); });
    _actionList.push_back([=]() { _dialogTextBox->FadeOut(1.0f); });
    _actionList.push_back([=]() { _dialogTextBox->FadeIn(1.0f); });
    _actionList.push_back([=]() { _dialogTextBox->FadeOut(1.0f); });
  }
}

void DialogUI::Update(float dt)
{
  UIPanel::Update(dt);

  
  
  if (isLClicked)
    _clickTimer -= dt;
  if (INPUT.IsKeyPress(MouseState::LB))
  {
    isLClicked = true;
    if (!isClickActioned)
    {
      NextStep();
      isClickActioned = true;
    }
  }
  if (_clickTimer <= 0)
  {
    _clickTimer = _clickThreshold;
    isLClicked = false;
    isClickActioned = false;
  }






  if (INPUT.IsKeyPress(Key::D1))
    _dialogTextBox->FadeIn(1.0f);
  if (INPUT.IsKeyPress(Key::D2))
    _Eliza->SetCurrentAnimSprite("Eliza_Resistance_Despair30");
  if (INPUT.IsKeyPress(Key::D3))
    _Eliza->SetCurrentAnimSprite("Eliza_Resistance_Despair70");
  if (INPUT.IsKeyPress(Key::D4))
    _Eliza->SetCurrentAnimSprite("Eliza_Noble");
  if (INPUT.IsKeyPress(Key::D5))
    _Eliza->SetCurrentAnimSprite("Eliza_Resistance_Sad");
  if (INPUT.IsKeyPress(Key::D6))
    _Eliza->SetCurrentAnimSprite("Eliza_Resistance_Joy");

  if (isCurrentActionFinished && _currentActionIndex <= _actionList.size() - 1)
  {
    isCurrentActionFinished = false;
    _actionList[_currentActionIndex]();
  }
}

void DialogUI::NextStep()
{
  _currentActionIndex++;
  if (_currentActionIndex >= _actionList.size())
    _currentActionIndex = _actionList.size() - 1;
  isCurrentActionFinished = true;
}

void DialogUI::PrevStep()
{
  _currentActionIndex--;
  if (_currentActionIndex < 0)
    _currentActionIndex = 0;
}
