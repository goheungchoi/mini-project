#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class GunfireButton : public UIPanel
{
public:
  GunfireButton(class World* world);
  virtual ~GunfireButton();
  void BeginLevel() override;
  void Update(float dt) override;

public:
  class UIAnim* _ellizaAnim{nullptr};
  bool _bGunFireUseFlag = false;

  class UICursor* _cursor{nullptr};
  class UIButton* _gunfireBtn{nullptr};
  class UIImage* _gunfireBtnImgs[3];

private:
  Vector2 _pos = {1750, 690};
  bool _bHover = false;
  void UpdateButtonState();
};