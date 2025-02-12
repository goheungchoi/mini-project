#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class MainMenuUI : public UIPanel
{
public:
  MainMenuUI(class World* world);
  virtual ~MainMenuUI();
  void BeginLevel() override;
  virtual void Update(float dt) override;
  void SetEndingFlag(int i);

private:
  class UIImage* _backGroundImg{nullptr};
  class UIImage* _hiddenBackGroundImg{nullptr};

  class UIImage* _LogoImage{nullptr};
  class UIImage* _hiddenLogoImage{nullptr};

  class UIImage* _startImg[2]{nullptr};
  class UIButton* _startBtn{nullptr};

  class UIImage* _challengeImg[2]{nullptr};
  class UIButton* _challengeBtn{nullptr};

  class UIImage* _exitImg[2]{nullptr};
  class UIButton* _exitBtn{nullptr};

  class UICursor* _cursor{nullptr};

  bool ending1 = false;
  bool ending2 = false;

  Vector2 _btnPos{440, 620};
  float _interval = 146.0f;
  bool _bHoverBtn = false;
};
