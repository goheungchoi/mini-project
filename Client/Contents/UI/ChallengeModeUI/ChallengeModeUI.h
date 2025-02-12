#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class ChallengeModeUI : public UIPanel
{
public:
  ChallengeModeUI(class World* world);
  virtual ~ChallengeModeUI();
  void Update(float dt) override;
  void BeginLevel() override;

private:
  bool _bHoverBtn = false;

  class World* _world;
  class UIImage* _backGroundImg{nullptr};

  class UICursor* _cursor{nullptr};
  class UIImage* _stage1Acitve{nullptr};
  class UIImage* _stage1DeAcitve{nullptr};
  class UIButton* _stage1Button{nullptr};
  class UIImage* _stage2Acitve{nullptr};
  class UIImage* _stage2DeAcitve{nullptr};
  class UIButton* _stage2Button{nullptr};
  class UIImage* _stage3Acitve{nullptr};
  class UIImage* _stage3DeAcitve{nullptr};
  class UIButton* _stage3Button{nullptr};
  class UIImage* _stage4Acitve{nullptr};
  class UIImage* _stage4DeAcitve{nullptr};
  class UIButton* _stage4Button{nullptr};
  class UIImage* _stage5Acitve{nullptr};
  class UIImage* _stage5DeAcitve{nullptr};
  class UIButton* _stage5Button{nullptr};
  class UIImage* _stage6Acitve{nullptr};
  class UIImage* _stage6DeAcitve{nullptr};
  class UIButton* _stage6Button{nullptr};
  class UIImage* _stage7Acitve{nullptr};
  class UIImage* _stage7DeAcitve{nullptr};
  class UIButton* _stage7Button{nullptr};
};
