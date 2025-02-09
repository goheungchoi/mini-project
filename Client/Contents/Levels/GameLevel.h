#pragma once

#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"

#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimationState.h"

#include "Contents/GameObjects/CameraObject.h"

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
  ModelHandle redCellModelHandle;
  ModelHandle greenCellModelHandle;

  ModelHandle mapBarMeshHandle;
  ModelHandle mapMuseumMeshHandle;
  ModelHandle mapWarehouseMeshHandle;

  ModelHandle animTestHandle;
  ModelHandle handGunHandle;
  
  ModelHandle OBsStoolHandle;
  ModelHandle OBsBox02Handle;

  

  CameraObject* testCamera;

  GameObject* pivot{nullptr};

  class Map* map{nullptr};
  class GameManager* gameManager{nullptr};

  class InGameUI* inGameUI{nullptr};
  class ResultDialogUI* resultDialogUI{nullptr};
  class TransitionUI* transitionUI;

  class DialogLevel* dialoglevel{nullptr};

public:
  GameLevel(const std::string& name) : Level(name) {}

  virtual void PrepareLevel() override;

  virtual void BeginLevel() override;

  // virtual void DestroyLevel() override { Level::DestroyLevel(); }
  virtual void CleanupLevel() override;
  eBattleResult GetBattleResult();
  void SetBattleResult(eBattleResult br);
  int stageIdx = 0;

protected:
  eBattleResult _battleResult;
  virtual void CreateMap();

  void TriggerAction();
};
