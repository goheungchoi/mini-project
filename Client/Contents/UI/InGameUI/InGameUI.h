#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class InGameUI : public UIPanel
{
public:
  InGameUI(class World* world);
  virtual ~InGameUI();

  void Update(float dt) override;

public:
  class UIImage* _gradientIMG{nullptr};

  class MainMission* _mainMission{nullptr};
  class SubMission* _subMission{nullptr};
  class PlayButton* _playBtn{nullptr};
  class GunfireButton* _gunfireBtn{nullptr};
  class AgentStorage* _agentStorage{nullptr};
  class RetryButton* _retryBtn{nullptr};
  class ApplyButton* _applyBtn{nullptr};

  class UICursor* _cursor{nullptr};

private:
  Vector2 animPos{-200, 300};
  const Vector2 DefaultPos{-200, 300};
  const Vector2 GoalPos{70, 300};
  const float animSpeed = 1000.0f;
  float elapsedTime = 0.0f;
  const float stopTime = 1.0f;
};
