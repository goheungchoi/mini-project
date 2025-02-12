#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class TutorialUI : public UIPanel
{
public:
  TutorialUI(class World* world);
  virtual ~TutorialUI();

  void Update(float dt) override;

public:
  class UIImage* _tutorialIMG{nullptr};
};
