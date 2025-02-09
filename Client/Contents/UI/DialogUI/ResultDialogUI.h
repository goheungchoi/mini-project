#pragma once
#include "DialogUI.h"

class ResultDialogUI : public DialogUI
{
  using Action = std::function<void(void)>;
public:
  ResultDialogUI(class World* world);
  virtual ~ResultDialogUI() = default;
};
