#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class InGameUI : public UIPanel
{
public:
  InGameUI(class World* world);
  virtual ~InGameUI();

private:
  class MainMission* _mainMission{nullptr};
  class SubMission* _subMission{nullptr};
  class PlayButton* _playBtn{nullptr};
  class GunfireButton* _gunfireBtn{nullptr};
  class AgentStorage* _agentStorage{nullptr};

  class UICursor* _cursor{nullptr};
};
