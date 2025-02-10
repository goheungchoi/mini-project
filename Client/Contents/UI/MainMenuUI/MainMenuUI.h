#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class MainMenuUI : public UIPanel
{
public:
  MainMenuUI(class World* world);
  virtual ~MainMenuUI();

  void SetEndingFlag(int i);

private:
  class UIImage* _backGroundImage{nullptr};
  class UIImage* _LogoImage{nullptr};
  class UIImage* _hiddenLogoImage{nullptr};

  class UIButton* _startBtn{nullptr};
  class UIImage* _startBtnImage{nullptr};
  class UIImage* _startBtnHoverImage{nullptr};
  class UIImage* _startBtnPressedImage{nullptr};

  class UICursor* _cursor{nullptr};

  bool ending1 = false;
  bool ending2 = false;
};
