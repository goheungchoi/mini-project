#pragma once
#include "GameFramework/Common.h"
#include "GameFramework/UI/UIElement/UIElement.h"

enum ButtonState
{
  BS_None,
  BS_Hovered,
  BS_Clicked,
  BS_Pressed
};

class UIButton : public UIElement
{
public:
  UIButton();
  virtual ~UIButton();


  private:
  ButtonState _state = BS_None;

  std::vector<std::function<void()>> onClickHandlers;
  std::vector<std::function<void()>> onHoveredHandlers;
  std::vector<std::function<void()>> onUnHoveredHandlers;
  std::vector<std::function<void()>> onPressedHandlers;

};
