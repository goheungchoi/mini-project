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
  UIButton(class World* world);
  virtual ~UIButton();

public:
  void Update(float dt) override;

  void AddOnClickHandler(std::function<void()> handler);
  void AddOnHoveredHandler(std::function<void()> handler);
  void AddOnUnHoveredHandler(std::function<void()> handler);
  void AddOnPressedHandler(std::function<void()> handler);

  void Render() override;
#ifdef _DEBUG
  void SetDebugDraw(bool debugFlag);
#endif

private:
  void OnClicked();
  void OnHovered();
  void OnUnHovered();
  void OnPressed();

private:
#ifdef _DEBUG
  bool bDebugDrawFlag = false;
#endif


  ButtonState _state = BS_None;

  std::vector<std::function<void()>> onClickHandlers;
  std::vector<std::function<void()>> onHoveredHandlers;
  std::vector<std::function<void()>> onUnHoveredHandlers;
  std::vector<std::function<void()>> onPressedHandlers;
};
