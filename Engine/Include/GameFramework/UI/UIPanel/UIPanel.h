#pragma once
#include "GameFramework/Common.h"
#include "GameFramework/UI/UIElement/UIElement.h"

class UIPanel : public UIElement
{
public:
  UIPanel(class World* world);
  virtual ~UIPanel();

  virtual void BeginLevel() override;
  virtual void Update(float dt) override;
  virtual void Render() override;
};
