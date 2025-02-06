#pragma once
#include "GameFramework\UI\UIPanel\UIPanel.h"
class DialogUI : public UIPanel
{
public:
  DialogUI(class World* world);
  virtual ~DialogUI() = default;

private:
  class UIAnim* _Eliza{nullptr};
  class UIText* _charactername{nullptr};
};
