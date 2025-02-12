#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class ChallengeModeUI : public UIPanel
{
public:
  ChallengeModeUI(class World* world);
  virtual ~ChallengeModeUI();

private:
  class UIImage* _backGroundImg{nullptr};
  class UICursor* _cursor{nullptr};
};
