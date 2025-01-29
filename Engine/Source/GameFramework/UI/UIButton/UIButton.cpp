#include "GameFramework/UI/UIButton/UIButton.h"
#include "Core/Input/InputSystem.h"

UIButton::UIButton(class World* world) : UIElement(world) {}

UIButton::~UIButton() {}

void UIButton::Update(float dt)
{
  Vector2 mousePos = {static_cast<float>(Input.GetCurrMouseState().x), static_cast<float>(Input.GetCurrMouseState().y)};

  int left = _position.x - (_size.x / 2);
  int top = _position.y - (_size.y / 2);
  int right = _position.x + (_size.x / 2);
  int bottom = _position.y + (_size.y / 2);

  if (left < mousePos.x && mousePos.x < right && top < mousePos.y &&
      mousePos.y < bottom)
  {
    if (Input.IsKeyUp(MouseState::LB) || Input.IsKeyUp(MouseState::RB) ||
        Input.IsKeyUp(MouseState::WM))
    {
      _state = BS_Clicked;
      OnClicked();
    }
    else if (Input.IsKeyDown(MouseState::LB) ||
             Input.IsKeyDown(MouseState::RB) || Input.IsKeyDown(MouseState::WM))
    {
      _state = BS_Pressed;
      OnPressed();
    }
    else
    {
      _state = BS_Hovered;
      OnHovered();
    }
  }
  else
  {
    if (_state == BS_Hovered || _state == BS_Pressed)
    {
      OnUnHovered();
    }
    _state = BS_None;
  }
}

void UIButton::AddOnClickHandler(std::function<void()> handler)
{
  onClickHandlers.push_back(handler);
}

void UIButton::AddOnHoveredHandler(std::function<void()> handler)
{
  onHoveredHandlers.push_back(handler);
}

void UIButton::AddOnUnHoveredHandler(std::function<void()> handler)
{
  onUnHoveredHandlers.push_back(handler);
}

void UIButton::AddOnPressedHandler(std::function<void()> handler)
{
  onPressedHandlers.push_back(handler);
}

void UIButton::OnClicked()
{
  for (auto handler : onClickHandlers)
  {
    handler();
  }
}

void UIButton::OnHovered()
{
  for (auto handler : onHoveredHandlers)
  {
    handler();
  }
}

void UIButton::OnUnHovered()
{
  for (auto handler : onUnHoveredHandlers)
  {
    handler();
  }
}

void UIButton::OnPressed()
{
  for (auto handler : onPressedHandlers)
  {
    handler();
  }
}
