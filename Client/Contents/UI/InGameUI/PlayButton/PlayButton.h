#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class PlayButton : public UIPanel
{
public:
  PlayButton(class World* world);
  virtual ~PlayButton();

  void Update(float dt) override;

  bool _bPlayflag = false;

private:
  class UIButton* _playBtn{nullptr};
  class UIImage* _playBtnImgs[3];

  class GunfireButton* _gunfireBtn{nullptr};
  class UIAnim* _elizaAnim{nullptr};

};
