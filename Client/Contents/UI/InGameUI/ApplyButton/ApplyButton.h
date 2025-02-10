#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class ApplyButton : public UIPanel
{
public:
  ApplyButton(class World* world);
  virtual ~ApplyButton();

  void Update(float dt) override;

private:
  class UIButton* _applyBtn{nullptr};
  class UIImage* _applyBtnImgs[2];
  bool _bHoverFlag = false;
};
