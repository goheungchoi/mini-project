#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class TestUI : public UIPanel
{
public:
  TestUI(class World* world);
  virtual ~TestUI();

private:
  class UIButton* testBtn{nullptr};
};
