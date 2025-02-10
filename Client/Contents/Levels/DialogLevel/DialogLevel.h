#pragma once
#include "GameFramework/World/World.h"
#include "GameFramework\Level\Level.h"
enum eBattleResult : int;

class DialogLevel : public Level
{
public:
  DialogLevel(const std::string& name) : Level(name) {}

  void PrepareLevel() override;
  void BeginLevel() override;
  void CleanupLevel() override;

  void SetStageIdx(int stageIdx);
  void SetBattleResult(eBattleResult battleresult);

private:
  eBattleResult _battleResult;
  int _stageidx;
  //class GameManager* gameManager{nullptr};
  class DialogUI* dialogUI{nullptr};
  class TransitionUI* transitionUI;
};
