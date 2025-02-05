#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class MainMenuUI : public UIPanel
{
public:
  MainMenuUI(class World* world);
  virtual ~MainMenuUI();

private:
  class UIImage* _backGroundImage{nullptr};
  class UIImage* _LogoImage{nullptr};

  class UIButton* _startBtn{nullptr};
  class UIImage* _startBtnImage{nullptr};
  class UIImage* _startBtnHoverImage{nullptr};
};
