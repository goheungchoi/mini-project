#pragma once
#include "GameFramework/Common.h"
#include "GameFramework/UI/UIElement/UIElement.h"

class UIPanel : public UIElement
{
public:
  UIPanel(class World* world);
  virtual ~UIPanel();

  void BeginLevel() override;
  void Update(float dt) override;
  void Render() override;
};
