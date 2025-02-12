#pragma once
#include "Contents/Levels/GameLevel.h"
#include "GameFramework\UI\UIPanel\UIPanel.h"

class AudioDramaUI : public UIPanel
{
public:
  AudioDramaUI(class World* world);
  virtual ~AudioDramaUI() = default;

  void BeginLevel() override;

  void Update(float dt) override;
  void Render() override;

  class UIImage* _Background{nullptr};
  class UIText* _AudioScript{nullptr};
  std::vector<class UIText*> _test;

  void PrintText(int stageidx, int scriptidx, float dt, float duration);
  void Skip();
  void SetTotalTime(float totaltime) { _totalTime = totaltime; };

  void Stage1(float dt);
  void Stage4(float dt);
  void Stage7(float dt);
  void EndingStage1(float dt);
  void EndingStage2(float dt);
  void RunStage(float dt);

  int stageidx = 1;


  bool PlayFlag = true;


  class MainMenuUI* mainmenuiptr{nullptr};

private:
  bool isEnding1 = false;
  bool isEnding2 = false;
  std::wstring _currentAudio;

  float _elapsedTimer = 0.f;
  float _totalTime = 0.f;
  bool fadeflag = true;

  std::wstring testScript = L"가나다라마바사아자차카타파하";
  struct ScriptInfo
  {
    std::wstring script;
    float time = 0.f;
    bool fadein = true;
    bool fadeout = true;
  };
  std::vector<std::vector<ScriptInfo>> _scripts;
};