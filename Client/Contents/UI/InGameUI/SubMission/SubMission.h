#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class SubMission : public UIPanel
{
public:
  SubMission(class World* world);
  virtual ~SubMission();

public:
  void SetText(std::wstring text, Vector2 pos = Vector2(290, 290));

private:
  class UIImage* _subMissionImg{nullptr};
  class UIText* _subMissionTxt{nullptr};
  class UIText* _selectTxt{nullptr};
};
