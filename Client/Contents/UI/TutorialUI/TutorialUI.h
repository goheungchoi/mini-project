#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class TutorialUI : public UIPanel
{
public:
  TutorialUI(class World* world);
  virtual ~TutorialUI();

  void Update(float dt) override;

public:
  class UIImage* _tutorialIMG[4]{nullptr};

  class UIImage* _nextIMG[2]{nullptr};
  class UIButton* _nextBtn{nullptr};

  class UIImage* _closeIMG[2]{nullptr};
  class UIButton* _closeBtn{nullptr};

  Vector2 _btnPos{960, 1000};

  bool _bHoverFlag = false;
  int curTutorialIdx = 0;

};
