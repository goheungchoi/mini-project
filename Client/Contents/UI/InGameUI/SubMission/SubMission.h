#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class SubMission : public UIPanel
{
public:
  SubMission(class World* world);
  virtual ~SubMission();

public:
  void SetText(std::wstring text);

private:
  class UIImage* _subMissionImg{nullptr};
  class UIText* _subMissionTxt{nullptr};
};
