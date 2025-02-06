#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class InGameUI : public UIPanel
{
public:
  InGameUI(class World* world);
  virtual ~InGameUI();

private:
  class UIImage* _mainMissonImg{nullptr};
  class UIText* _mainMissonTxt{nullptr};

  class UIImage* _subMissonImg{nullptr};
  class UIText* _subMissonTxt{nullptr};

  class UIButton* _playBtn{nullptr};
  class UIImage* _playBtnImgList[3];

  class UIButton* _snipingBtn{nullptr};
  class UIImage* _snipingBtnImg{nullptr};

  class UIButton* _combatBtn{nullptr};
  class UIImage* _combatBtnImg{nullptr};
};
