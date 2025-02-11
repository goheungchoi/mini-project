#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

class MainMission : public UIPanel
{
public:
  MainMission(class World* world);
  virtual ~MainMission();

public:
  void SetText(std::wstring text);

private:
  class UIImage* _mainMissionImg{nullptr};
  class UIText* _mainMissionTxt{nullptr};
  //class UIText* _selectTxt{nullptr};

  Vector2 _pos = {338, 120};
};
