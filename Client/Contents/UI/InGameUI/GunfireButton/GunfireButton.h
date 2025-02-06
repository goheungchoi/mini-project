#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class GunfireButton : public UIPanel
{
public:
  GunfireButton(class World* world);
  virtual ~GunfireButton();

private:
  class UIButton* _gunfireBtn{nullptr};
  class UIImage* _gunfireBtnImgs[3];
};
