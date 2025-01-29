#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class TestUI : public UIPanel
{
public:
  TestUI(class World* world) : UIPanel(world) {}
  virtual ~TestUI() = default;

  virtual void BeginLevel() override {}
  virtual void Update(float dt) override {}
  virtual void Render() override {}
};
