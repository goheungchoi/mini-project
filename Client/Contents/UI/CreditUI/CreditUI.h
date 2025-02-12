#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class CreditUI : public UIPanel
{
public:
  CreditUI(class World* world);
  virtual ~CreditUI();
  void BeginLevel() override;
  virtual void Update(float dt) override;

private:
  class UICursor* _cursor{nullptr};
  class UIImage* _endingIMG{nullptr};
  class UIImage* _backIMG{nullptr};
  class UIImage* _vignetteIMG{nullptr};
  Vector2 _IMGpos{960, 1500};
  float _speed = 130.0f;
};
