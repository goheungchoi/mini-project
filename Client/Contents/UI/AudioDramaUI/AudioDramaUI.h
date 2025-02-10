#pragma once
#include "Contents/Levels/GameLevel.h"
#include "GameFramework\UI\UIPanel\UIPanel.h"

class AudioDramaUI : public UIPanel
{
public:
  AudioDramaUI(class World* world);
  virtual ~AudioDramaUI() = default;

  void Update(float dt) override;

  class UIImage* _Background{nullptr};
  class UIText* _AudioScript{nullptr};

  void PrintText(int stageidx, int scriptidx, float dt, float duration);

  void SetTotalTime(float totaltime) { _totalTime = totaltime; };

  void Stage1(float dt);
  void Stage4(float dt);
  void Stage7(float dt);
  void EndingStage1(float dt);
  void EndingStage2(float dt);
  void RunStage(float dt);


private:
  int stageidx;
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