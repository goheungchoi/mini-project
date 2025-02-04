#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class TestUI : public UIPanel
{
public:
  TestUI(class World* world);
  virtual ~TestUI();

  virtual void Render() override;

private:
  class UIButton* testBtn{nullptr};
  class UIImage* testIMG{nullptr};
  class UIText* testTxt{nullptr};
};
