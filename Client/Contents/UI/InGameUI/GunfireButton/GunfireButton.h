#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class GunfireButton : public UIPanel
{
public:
  GunfireButton(class World* world);
  virtual ~GunfireButton();

  void Update(float dt) override;

private:
  class UICursor* _cursor{nullptr};
  class UIButton* _gunfireBtn{nullptr};
  class UIImage* _gunfireBtnImgs[3];
  bool _bGunFireUseFlag = false;
};
