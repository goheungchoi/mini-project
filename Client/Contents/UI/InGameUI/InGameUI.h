#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class InGameUI : public UIPanel
{
public:
  InGameUI(class World* world);
  virtual ~InGameUI();

  void Update(float dt) override;

private:
  class UIImage* _mainMissonImg{nullptr};
  class UIText* _mainMissonTxt{nullptr};

  class UIImage* _subMissonImg{nullptr};
  class UIText* _subMissonTxt{nullptr};

  class UIButton* _playBtn{nullptr};
  class UIImage* _playBtnImgs[3];

  class UIButton* _orderBtn{nullptr};
  class UIImage* _orderBtnImgs[3];

  class UIButton* _combatBtn{nullptr};
  class UIImage* _combatBtnImg{nullptr};

  class UICursor* _cursor{nullptr};

  class Map* _map{};
  bool _bPlayflag = false;
};
