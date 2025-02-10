#pragma once

#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"

#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimationState.h"

#include "Contents/GameObjects/CameraObject.h"
#include "DialogLevel/DialogLevel.h"

#include "Contents/GameObjects/Map/FixedCamera/FixedCamera.h"
enum eBattleResult
{
  PerfectWin,
  CivilDeadWin,
  AllyDeadWin,
  BothDeadWin,
  AllyDeadLose,
  Lose
};

class GameLevel : public Level
{
protected:
  // Resource handles
  ModelHandle mapBarMeshHandle;
  ModelHandle mapMuseumMeshHandle;
  ModelHandle mapWarehouseMeshHandle;

	ModelHandle obstacleBox01;
	ModelHandle obstacleBox02;
	ModelHandle obstacleDrum;
	ModelHandle obstacleDrumOld;
	ModelHandle obstacleLion;
	ModelHandle obstacleSofa;
	ModelHandle obstacleStool;
	ModelHandle obstacleVBox;

  //CameraObject* testCamera;
  FixedCamera* testCamera;
  GameObject* pivot{nullptr};

  class Map* map{nullptr};
  class GameManager* gameManager{nullptr};

  class InGameUI* inGameUI{nullptr};
  class ResultDialogUI* resultDialogUI{nullptr};
  class TransitionUI* transitionUI;

  class DialogLevel* dialoglevel{nullptr};

  Vector2 AgentPos{1790, 925};
  float spacing = 180;

public:
  GameLevel(const std::string& name) : Level(name) {}

  virtual void PrepareLevel() override;

  virtual void BeginLevel() override;

  // virtual void DestroyLevel() override { Level::DestroyLevel(); }
  virtual void CleanupLevel() override;
  void BindDialogLevel(class DialogLevel* dialoglevelptr);
  eBattleResult GetBattleResult();
  int GetStageIdx() { return stageIdx; }
  void SetBattleResult(eBattleResult br);
  void SetStageIdx(int _stageIdx)
  {
    stageIdx = _stageIdx;
    dialoglevel->SetStageIdx(_stageIdx);

  };

protected:
  int stageIdx = 0;
  eBattleResult _battleResult;
  virtual void CreateMap();

  void TriggerAction();

};
