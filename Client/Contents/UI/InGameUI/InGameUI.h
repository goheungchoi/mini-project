#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class InGameUI : public UIPanel
{
public:
  InGameUI(class World* world);
  virtual ~InGameUI();

  void Update(float dt) override;
  void SetBGM();

public:
  class UIImage* _gradientIMG{nullptr};

  class MainMission* _mainMission{nullptr};
  class SubMission* _subMission[2] {nullptr};
  class PlayButton* _playBtn{nullptr};
  class GunfireButton* _gunfireBtn{nullptr};
  class AgentStorage* _agentStorage{nullptr};
  class RetryButton* _retryBtn{nullptr};
  class ApplyButton* _applyBtn{nullptr};

  class UICursor* _cursor{nullptr};

  bool fadeflag = true;
  Vector2 animPos{-600, 300};
  const Vector2 GoalPos{70, 300};

  const float delayTime{1.f};
  float delayElapsedTime{0.f};

  bool isActionStarted{false};

private:
  const Vector2 DefaultPos{-600, 300};
  const float animSpeed = 5000.0f;
  float elapsedTime = 0.0f;
  const float stopTime = 2.0f;

  int levelIdx = 0;
  int prevLevelIdx = 0;
};
