#pragma once
#include "GameFramework/UI/UIPanel/UIPanel.h"

enum class CursorType
{
  DEFAULT,
  SKILL
};

class UICursor : public UIPanel
{
public:
  UICursor(class World* world);
  virtual ~UICursor();

public:
  void Update(float dt) override;
  void Render() override;
  void SetCursorType(CursorType cursorType);

private:
  class Sprite* _sprites[2]{};
  int _selectedIndex = 0;
};
