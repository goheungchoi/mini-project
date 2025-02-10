#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class RetryButton : public UIPanel
{
public:
  RetryButton(class World* world);
  virtual ~RetryButton();

  void Update(float dt) override;

private:
  class UIButton* _retryBtn{nullptr};
  class UIImage* _retryBtnImgs[2];
  bool _bHoverFlag = false;
};
