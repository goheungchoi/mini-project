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

  ModelHandle defaultCellModelHandle;
  ModelHandle placementCellModelHandle;
  ModelHandle rangeCellModelHandle;
  ModelHandle dashCellModelHandle;
  ModelHandle damageCellModelHandle;

  TextureHandle deathIndicatorHandle;
  TextureHandle tabIndicatorHandle;

	ModelHandle allyDirectionIndicatorModelHandle;
  ModelHandle enemyDirectionIndicatorModelHandle;

  TextureHandle brawlerInactiveIndicatorTextureHandle;
  TextureHandle brawlerActiveIndicatorTextureHandle;
  TextureHandle slasherInactiveIndicatorTextureHandle;
  TextureHandle slasherActiveIndicatorTextureHandle;
  TextureHandle gunmanInactiveIndicatorTextureHandle;
  TextureHandle gunmanActiveIndicatorTextureHandle;

  ModelHandle enemyBrawlerModelHandle;
  ModelHandle enemyGunmanModelHandle;
  ModelHandle enemyBossModelHandle;

  ModelHandle civilianModelHandle;
  ModelHandle elizaModelHandle;

  ModelHandle allyBrawlerModelHandle;
  ModelHandle allySlasherModelHandle;
  ModelHandle allyGunmanModelHandle;

  ModelHandle characterIndicatorModelHandle;

  ModelHandle obstacleBox01ModelHandle;
  ModelHandle obstacleBox02ModelHandle;
  ModelHandle obstacleDrumModelHandle;
  ModelHandle obstacleDrumOldModelHandle;
  ModelHandle obstacleLionModelHandle;
  ModelHandle obstacleSofaModelHandle;
  ModelHandle obstacleStoolModelHandle;
  ModelHandle obstacleVBoxModelHandle;

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
