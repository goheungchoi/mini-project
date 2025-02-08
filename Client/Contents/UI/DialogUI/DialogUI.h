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
  class UIText* _speakerNameText {nullptr};
  class UIText* _speakerClassText {nullptr};

  class UIImage* _dialogTextBox{nullptr};
  class UIImage* _dialogBtnImage{nullptr};
  class UIButton* _dialogButton{nullptr};
  class UIText* _dialogText {nullptr};

  bool bInGame = false;

  void Update(float dt) override;

  void NextStep();
  void PrevStep();

  void ParseDialogScript();
  void SetStageDialogIndex(int StageIdx);


private:
  struct dialogInfo
  {
    int idx;
    bool IsElizaSpeaking = true;
    std::wstring dialogtext;
    std::string ElizaAnim;
  };
  std::vector<dialogInfo> _dialogList;
  std::vector<std::string> _csvlist;
  const wchar_t* _nameEliza = L"¿¤¸®ÀÚ";
  const wchar_t* _namePlayer = L"´ç½Å";


  bool isCurrentActionFinished = true;
  std::vector<Action> _actionList;
  UINT _currentActionIndex = 0;

    bool isLClicked = false;
  bool isClickActioned = false;
  float _clickThreshold = .2f;
  float _clickTimer = .2f;
};
