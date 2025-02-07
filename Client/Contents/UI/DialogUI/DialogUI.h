#pragma once
#include "GameFramework\UI\UIPanel\UIPanel.h"
class DialogUI : public UIPanel
{
  using Action = std::function<void(void)>;
public:
  DialogUI(class World* world);
  virtual ~DialogUI() = default;

  class UIImage* _PhotoPanel{nullptr};

  class UIAnim* _Eliza{nullptr};

  class UIImage* _dialogTextBox{nullptr};
  class UIImage* _dialogBtnImage{nullptr};
  class UIButton* _dialogButton{nullptr};

  bool bInGame = false;

  void Update(float dt) override;

  void NextStep();
  void PrevStep();

private:
  bool isCurrentActionFinished = true;
  std::vector<Action> _actionList;
  UINT _currentActionIndex = 0;

    bool isLClicked = false;
  bool isClickActioned = false;
  float _clickThreshold = .2f;
  float _clickTimer = .2f;
};
