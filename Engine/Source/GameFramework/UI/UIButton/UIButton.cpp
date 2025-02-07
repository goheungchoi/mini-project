#include "GameFramework/UI/UIButton/UIButton.h"
#include "Core/Input/InputSystem.h"

UIButton::UIButton(class World* world) : UIElement(world) {}

UIButton::~UIButton() {}

void UIButton::Update(float dt)
{
  Vector2 mousePos = {static_cast<float>(INPUT.GetCurrMouseState().x),
                      static_cast<float>(INPUT.GetCurrMouseState().y)};

  int left = _position.x;
  int top = _position.y;
  int right = _position.x + _size.x;
  int bottom = _position.y + _size.y;

  if (left < mousePos.x && mousePos.x < right && top < mousePos.y &&
      mousePos.y < bottom)
  {
    if (INPUT.IsKeyUp(MouseState::LB) || INPUT.IsKeyUp(MouseState::RB) ||
        INPUT.IsKeyUp(MouseState::WM))
    {
      _state = BS_Clicked;
      OnClicked();
    }
    else if (INPUT.IsKeyDown(MouseState::LB) ||
             INPUT.IsKeyDown(MouseState::RB) || INPUT.IsKeyDown(MouseState::WM))
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

void UIButton::Render()
{
  #ifdef _DEBUG
  if (bDebugDrawFlag)
  {
    _world->_renderer->DrawRectangle(Color(1, 0, 0, 1),
                                     Vector4(_position.x, _position.y,
                                             _position.x + _size.x,
                                             _position.y + _size.y));
  }
  #endif
}
#ifdef _DEBUG
void UIButton::SetDebugDraw(bool debugFlag)
{
  bDebugDrawFlag = debugFlag;
}
#endif

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
