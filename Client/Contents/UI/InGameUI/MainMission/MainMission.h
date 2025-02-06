#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class MainMission : public UIPanel
{
public:
  MainMission(class World* world);
  virtual ~MainMission();

private:
  class UIImage* _mainMissionImg{nullptr};
  class UIText* _mainMissionTxt{nullptr};
};
