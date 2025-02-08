#pragma once
#include "GameFramework\UI\UIPanel\UIPanel.h"

enum eBattleResult
{
    PerfectWin,
    CivilDeadWin,
    AllyDeadWin
};



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
  class UIText* _dialogText {nullptr};
  class UIImage* _dialogBtnImage{nullptr};
  class UIButton* _dialogButton{nullptr};

  class UIImage* _playerSelectBtnImage1{nullptr};
  class UIButton* _playerSelectButton1{nullptr};
  class UIText* _playerSelectBtnText1{nullptr};

  class UIImage* _playerSelectBtnImage2{nullptr};
  class UIButton* _playerSelectButton2{nullptr};
  class UIText* _playerSelectBtnText2{nullptr};

  bool bInGame = false;
  bool bPlayerSelection = false;

  void Update(float dt) override;

  void NextStep();
  void PrevStep();

  void ParseDialogScript();
  void SetStageDialogIndex(int StageIdx);
  void SetPrevBattleResult(eBattleResult result)
  { _prevBattleResult = result;
  }
  void ElizaDialogStep(int idx);
  void PlayerSelectDialogStep();

private:

  eBattleResult _prevBattleResult;
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

  //  bool isLClicked = false;
  //bool isClickActioned = false;
  //float _clickThreshold = .2f;
  //float _clickTimer = .2f;
};
