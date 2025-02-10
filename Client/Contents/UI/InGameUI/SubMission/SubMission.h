#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class SubMission : public UIPanel
{
public:
  SubMission(class World* world, Vector2 pos = Vector2(208, 252));
  virtual ~SubMission();

  void Update(float dt) override;

public:
  void SetText(std::wstring text);

private:
  class UIImage* _subMissionImg{nullptr};
  class UIImage* _failImg{nullptr};
  class UIText* _subMissionTxt{nullptr};
  class UIText* _selectTxt{nullptr};

  class RetryButton* RetryBtn{nullptr};
  Vector2 _pos = {208, 252};
  int StageIdx = 0;
};
