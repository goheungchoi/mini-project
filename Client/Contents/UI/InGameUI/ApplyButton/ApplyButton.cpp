#include "ApplyButton.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/Levels/GameLevel.h"
#include "Contents/UI/AudioDramaUI/AudioDramaUI.h"
#include "Contents/UI/DialogUI/ResultDialogUI.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "GameFramework/UI/UIImage/UIImage.h"

#include "Contents/SoundList/SoundList.h"
#include "SoundSystem/SoundManager.h"

ApplyButton::ApplyButton(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();
  _applyBtnImgs[0] = CreateUI<UIImage>(L"ApplyBtnImg_Act");
  _applyBtnImgs[1] = CreateUI<UIImage>(L"ApplyBtnImg_Hover");

  _applyBtnImgs[0]->SetSprite("2D\\UI\\UI_Confirm_Act.png", {1780, 140});
  _applyBtnImgs[1]->SetSprite("2D\\UI\\UI_Confirm_Hover.png", {1780, 140});
  _applyBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);

  _applyBtn = CreateUI<UIButton>(L"ApplyBtn");
  _applyBtn->SetSize(_applyBtnImgs[0]->GetSize());
  _applyBtn->SetCenterPos({1780, 140});

  SetOnActivatedEvent([=]() {
    _applyBtn->Activate();
    _applyBtnImgs[0]->Activate();
    _applyBtnImgs[1]->Activate();
  });

#ifdef _DEBUG
  _applyBtn->SetDebugDraw(true);
#endif // _DEBUG

  _applyBtn->AddOnClickHandler([=]() {
    
    SoundManager::PlaySound(SoundList::Button_Click);

    // Challenge 모드 일 때, 예외처리
    int curLevelIdx =
        static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx();

    if (curLevelIdx == 10 || curLevelIdx == 11 || curLevelIdx == 12 ||
        curLevelIdx == 13 || curLevelIdx == 14 || curLevelIdx == 15 ||
        curLevelIdx == 16)
    {
      world->PrepareChangeLevel("Challenge Level");
      world->CommitLevelChange();

      _bHoverFlag = false;

      return;
    }

    if (!_map->isPlacementModeOn)
    {
      _world->_canvas->HidePanel(L"InGameUI");

      if (_map->IsGameFinished())
      {
        static_cast<GameLevel*>(_world->GetCurrentLevel())
            ->SetBattleResult(eBattleResult::PerfectWin);
        if (world->GetCurrentLevel()->name == "Level7")
        {
          world->_canvas->GetPanel<ResultDialogUI>(L"ResultDialogUI")
              ->bIsElizaDead = true;
          if (_map->GetNumEnemies() != _map->GetNumDeadEnemies())
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::Lose);

          else if (_map->GetNumDeadAllies() >= 3)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::AllyDeadLose);

          else if (_map->GetNumDeadAllies() > 0 &&
                   _map->GetNumDeadCivilians() > 0)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::BothDeadWin);

          else if (_map->GetNumDeadAllies() <= 0 &&
                   _map->GetNumDeadCivilians() > 0)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::CivilDeadWin);

          else if (_map->GetNumDeadAllies() > 0 &&
                   _map->GetNumDeadCivilians() <= 0)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::AllyDeadWin);
          else
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::PerfectWin);
        }
        else if (world->GetCurrentLevel()->name == "Level8")
        {
          world->_canvas->GetPanel<ResultDialogUI>(L"ResultDialogUI")
              ->bIsElizaDead = true;
          if (_map->GetNumEnemies() != _map->GetNumDeadEnemies())
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::Lose);

          else if (_map->GetNumDeadAllies() >= 3)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::AllyDeadLose);

          else if (_map->GetNumDeadAllies() > 0 &&
                   _map->GetNumDeadCivilians() > 0)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::BothDeadWin);

          else if (_map->GetNumDeadAllies() <= 0 &&
                   _map->GetNumDeadCivilians() > 0)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::CivilDeadWin);

          else if (_map->GetNumDeadAllies() > 0 &&
                   _map->GetNumDeadCivilians() <= 0)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::AllyDeadWin);
          else
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::PerfectWin);
        }
        else if (world->GetCurrentLevel()->name == "Level8_2")
        {
          world->_canvas->GetPanel<ResultDialogUI>(L"ResultDialogUI")
              ->bIsElizaDead = false;

          if (_map->GetNumEnemies() != _map->GetNumDeadEnemies())
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::Lose);

          else if (_map->GetNumDeadAllies() >= 3)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::AllyDeadLose);

          else if (_map->GetNumDeadAllies() > 0 &&
                   _map->GetNumDeadCivilians() > 0)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::BothDeadWin);

          else if (_map->GetNumDeadAllies() <= 0 &&
                   _map->GetNumDeadCivilians() > 0)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::CivilDeadWin);

          else if (_map->GetNumDeadAllies() > 0 &&
                   _map->GetNumDeadCivilians() <= 0)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::AllyDeadWin);
          else
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::PerfectWin);


        }

        else
        {
          if (_map->GetNumEnemies() != _map->GetNumDeadEnemies())
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::Lose);

          else if (_map->GetNumDeadAllies() >= 3)
            static_cast<GameLevel*>(world->GetCurrentLevel())
                ->SetBattleResult(eBattleResult::AllyDeadLose);

          else if (_map->GetNumCivilians() > 0)
          {
            if (_map->GetNumDeadAllies() > 0 && _map->GetNumDeadCivilians() > 0)
              static_cast<GameLevel*>(world->GetCurrentLevel())
                  ->SetBattleResult(eBattleResult::BothDeadWin);
            else if (_map->GetNumDeadAllies() <= 0 &&
                     _map->GetNumDeadCivilians() > 0)
              static_cast<GameLevel*>(world->GetCurrentLevel())
                  ->SetBattleResult(eBattleResult::CivilDeadWin);

            else if (_map->GetNumDeadAllies() > 0 &&
                     _map->GetNumDeadCivilians() <= 0)
              static_cast<GameLevel*>(world->GetCurrentLevel())
                  ->SetBattleResult(eBattleResult::AllyDeadWin);
            else
              static_cast<GameLevel*>(world->GetCurrentLevel())
                  ->SetBattleResult(eBattleResult::PerfectWin);
          }

          else if (_map->GetNumCivilians() <= 0)
          {

            if (_map->GetNumDeadAllies() >= 3)
              static_cast<GameLevel*>(world->GetCurrentLevel())
                  ->SetBattleResult(eBattleResult::AllyDeadLose);

            else if (_map->GetNumDeadAllies() > 0)
              static_cast<GameLevel*>(world->GetCurrentLevel())
                  ->SetBattleResult(eBattleResult::AllyDeadWin);

            else
              static_cast<GameLevel*>(world->GetCurrentLevel())
                  ->SetBattleResult(eBattleResult::PerfectWin);
          }
        }

        //cheat code
        if (perfectwinflag)
          static_cast<GameLevel*>(_world->GetCurrentLevel())
              ->SetBattleResult(eBattleResult::PerfectWin);

        if (static_cast<GameLevel*>(world->GetCurrentLevel())
                ->GetBattleResult() != eBattleResult::PerfectWin)
        {
          _world->_canvas->ShowPanel(L"ResultDialogUI");
        }
        else
        {

          if (static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx() ==
              8)
          {
            _world->_canvas->GetPanel<AudioDramaUI>(L"AudioDramaUI")->stageidx =
                8;
            _world->_canvas->GetPanel<AudioDramaUI>(L"AudioDramaUI")->PlayFlag =
                true;
            _world->_canvas->GetPanel<InGameUI>(L"InGameUI")->Deactivate();
            _world->_canvas->GetPanel<AudioDramaUI>(L"AudioDramaUI")
                ->Activate();
          }
          else if (static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx() ==
              9)
          {
            _world->_canvas->GetPanel<AudioDramaUI>(L"AudioDramaUI")->stageidx =
                9;
            _world->_canvas->GetPanel<AudioDramaUI>(L"AudioDramaUI")->PlayFlag =
                true;
            _world->_canvas->GetPanel<InGameUI>(L"InGameUI")->Deactivate();
            _world->_canvas->GetPanel<AudioDramaUI>(L"AudioDramaUI")
                ->Activate();
          }
          else
          {

            static_cast<GameLevel*>(_world->_currentLevel)
                ->SetBattleResult(static_cast<GameLevel*>(_world->_currentLevel)
                                      ->GetBattleResult());
            static_cast<GameLevel*>(_world->_currentLevel)
                ->SetStageIdx(static_cast<GameLevel*>(_world->_currentLevel)
                                  ->GetStageIdx());
            world->PrepareChangeLevel("Dialog Level");
            world->CommitLevelChange();
          }
        }

      }
      _bHoverFlag = false;
    }
  });


  _applyBtn->AddOnHoveredHandler([this]() {
    if (!_bHoverFlag)
    {
      SoundManager::PlaySound(SoundList::Button_Hover);
      _bHoverFlag = true;
    }
  });

  _applyBtn->AddOnUnHoveredHandler([this]() { _bHoverFlag = false; });

}

ApplyButton::~ApplyButton() {}

void ApplyButton::Update(float dt)
{
  __super::Update(dt);

  if (!_bHoverFlag)
  {
    _applyBtnImgs[0]->SetStatus(EStatus::EStatus_Active);
    _applyBtnImgs[1]->SetStatus(EStatus::EStatus_Inactive);
  }
  else
  {
    _applyBtnImgs[0]->SetStatus(EStatus::EStatus_Inactive);
    _applyBtnImgs[1]->SetStatus(EStatus::EStatus_Active);
  }
}
