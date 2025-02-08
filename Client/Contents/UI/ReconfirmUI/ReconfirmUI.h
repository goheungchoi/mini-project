#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class ReconfirmUI : public UIPanel
{
public:
  ReconfirmUI(class World* world);
  virtual ~ReconfirmUI();

private:
  class UIImage* _testImg;
};
