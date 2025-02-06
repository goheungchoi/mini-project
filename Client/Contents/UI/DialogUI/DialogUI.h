#pragma once
#include "GameFramework\UI\UIPanel\UIPanel.h"
class DialogUI : public UIPanel
{
public:
  DialogUI(class World* world);
  virtual ~DialogUI() = default;

  class UIImage* _PhotoPanel{nullptr};

  class UIAnim* _Eliza{nullptr};


  bool bInGame = false;

  void Update(float dt) override;
private:
};
