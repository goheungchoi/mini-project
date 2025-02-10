#include "ApplyButton.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/Levels/GameLevel.h"
#include "Contents/UI/DialogUI/ResultDialogUI.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"

ApplyButton::ApplyButton(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();
  _applyBtnImgs[0] = CreateUI<UIImage>(L"ApplyBtnImg_Act");
  _applyBtnImgs[0]->SetSprite("2D\\UI\\UI_Confirm_Act.png", {1800, 100});

  _applyBtn = CreateUI<UIButton>(L"ApplyBtn");
  _applyBtn->SetSize(_applyBtnImgs[0]->GetSize());
  _applyBtn->SetCenterPos({1800, 100});
#ifdef _DEBUG
  _applyBtn->SetDebugDraw(true);
#endif // _DEBUG

  _applyBtn->AddOnClickHandler([=]() {
    //_bApplyflag = true;
    _world->_canvas->HidePanel(L"InGameUI");

    if (_map->IsGameFinished())
    {
      //if (world->GetCurrentLevel()->name == "Level6")
      //{
      //  if (_map->GetNumEnemies() != _map->GetNumDeadEnemies())
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::Lose);
      //  else if (_map->GetNumDeadAllies() >= 3)
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::AllyDeadLose);
      //  else if (_map->GetNumDeadAllies() > 1 &&
      //           _map->GetNumDeadCivilians() > 0)
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::BothDeadWin);
      //  else if (_map->GetNumDeadAllies() <= 1 &&
      //           _map->GetNumDeadCivilians() > 0)
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::CivilDeadWin);
      //  else if (_map->GetNumDeadAllies() > 1 &&
      //           _map->GetNumDeadCivilians() <= 0)
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::AllyDeadWin);
      //  else if (_map->GetNumDeadAllies() <= 1 &&
      //           _map->GetNumDeadCivilians() <= 0)
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::PerfectWin);
      //}
      //else if (world->GetCurrentLevel()->name == "Level7")
      //{
      //  if (_map->GetNumEnemies() != _map->GetNumDeadEnemies())
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::Lose);
      //  else if (_map->GetNumDeadAllies() >= 3)
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::AllyDeadLose);
      //  else if (_map->GetNumDeadCivilians() > 0 &&
      //           _map->GetNumDeadAllies() < 2)
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::CivilDeadWin);
      //  else if (_map->GetNumDeadCivilians() <= 0 &&
      //           _map->GetNumDeadAllies() >= 2)
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::AllyDeadWin);
      //}
      //else
      //{
      //  if (_map->GetNumEnemies() != _map->GetNumDeadEnemies())
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::Lose);
      //  else if (_map->GetNumDeadAllies() >= 3)
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::AllyDeadLose);
      //  else if (_map->GetNumDeadAllies() > 0 &&
      //           _map->GetNumDeadCivilians() > 0)
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::BothDeadWin);
      //  else if (_map->GetNumDeadAllies() <= 0 &&
      //           _map->GetNumDeadCivilians() > 0)
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::CivilDeadWin);
      //  else if (_map->GetNumDeadAllies() > 0 &&
      //           _map->GetNumDeadCivilians() <= 0)
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::AllyDeadWin);
      //  else
      //    static_cast<GameLevel*>(world->GetCurrentLevel())
      //        ->SetBattleResult(eBattleResult::PerfectWin);
      //}

      if (static_cast<GameLevel*>(world->GetCurrentLevel())
              ->GetBattleResult() != eBattleResult::PerfectWin)
      {
        _world->_canvas->ShowPanel(L"ResultDialogUI");
        
      }
      else
      {
        static_cast<GameLevel*>(_world->_currentLevel)
            ->SetBattleResult(static_cast<GameLevel*>(_world->_currentLevel)->GetBattleResult());
        static_cast<GameLevel*>(_world->_currentLevel)
            ->SetStageIdx(static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx());
        world->PrepareChangeLevel("Dialog Level");
        world->CommitLevelChange();
      }
    }
  });


  _applyBtn->AddOnHoveredHandler(
      [this]() { 
      
      });

  _applyBtn->AddOnUnHoveredHandler(
      [this]() { 
      
      });

}

ApplyButton::~ApplyButton() {}

void ApplyButton::Update(float dt)
{
  __super::Update(dt);
}
