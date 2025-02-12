#include "InGameUI.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Contents/Levels/GameLevel.h"
#include "Contents/UI/DialogUI/ResultDialogUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"

#include "Contents/UI/InGameUI/MainMission/MainMission.h"
#include "Contents/UI/InGameUI/SubMission/SubMission.h"
#include "Contents/UI/InGameUI/PlayButton/PlayButton.h"
#include "Contents/UI/InGameUI/RetryButton/RetryButton.h"
#include "Contents/UI/InGameUI/ApplyButton/ApplyButton.h"
#include "Contents/UI/InGameUI/GunfireButton/GunfireButton.h"
#include "Contents/UI/InGameUI/AgentStorage/AgentStorage.h"
#include "GameFramework/UI/UICursor/UICursor.h"
#include "GameFramework/UI/UIAnim/UIAnim.h"
#include "GameFramework/UI/UIImage/UIImage.h"

InGameUI::InGameUI(World* world) : UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();
  
  levelIdx = static_cast<GameLevel*>(_world->GetCurrentLevel())->GetStageIdx();

  _mainMission = CreateUI<MainMission>(L"MainMission");
  _subMission[0] = CreateUI<SubMission>(L"SubMission"); // 엘리자 희생
  _subMission[1] = CreateUI<SubMission>(L"SubMission_2", Vector2(208, 340));  // 대원 2명 희생

  _playBtn = CreateUI<PlayButton>(L"PlayBtn");
  _retryBtn = CreateUI<RetryButton>(L"RetryBtn");
  _applyBtn = CreateUI<ApplyButton>(L"ApplyBtn");

  _gunfireBtn = CreateUI<GunfireButton>(L"GunfireBtn");

  _agentStorage = CreateUI<AgentStorage>(L"AgentStorage");

  _cursor = _world->_canvas->GetPanel<UICursor>(L"Cursor");
  _cursor->SetCursorType(CursorType::DEFAULT);

  _gradientIMG = CreateUI<UIImage>(L"GradientIMG");
  _gradientIMG->SetSprite("2D\\UI\\gradient.png", {1800, 980});
  _gradientIMG->SetScale({0.7f, 0.7f});
  SetOnActivatedEvent([=]() { _playBtn->Activate(); });

  
}

InGameUI::~InGameUI() {}

void InGameUI::Update(float dt)
{
  __super::Update(dt);
  
  /*if (INPUT.IsKeyDown(Key::F1))
  {

      _world->_canvas->GetPanel<ResultDialogUI>(L"ResultDialogUI")
          ->bIsElizaDead = true;
  }
  if (INPUT.IsKeyPress(Key::F2))
    static_cast<GameLevel*>(_world->GetCurrentLevel())
        ->SetBattleResult(eBattleResult::CivilDeadWin);
  if (INPUT.IsKeyPress(Key::F3))
    static_cast<GameLevel*>(_world->GetCurrentLevel())
        ->SetBattleResult(eBattleResult::AllyDeadWin);
  if (INPUT.IsKeyPress(Key::F4))
    static_cast<GameLevel*>(_world->GetCurrentLevel())
        ->SetBattleResult(eBattleResult::BothDeadWin);
  if (INPUT.IsKeyPress(Key::F5))
    static_cast<GameLevel*>(_world->GetCurrentLevel())
        ->SetBattleResult(eBattleResult::AllyDeadLose);
  if (INPUT.IsKeyPress(Key::F6))
    static_cast<GameLevel*>(_world->GetCurrentLevel())
        ->SetBattleResult(eBattleResult::Lose);*/


  // ElizaAnim 이 Active상태이면 위치 움직임
  UIAnim* _elizaAnim = _gunfireBtn->GetUI<UIAnim>(L"ElizaAnim");

  if (_elizaAnim)
  {
    if (_elizaAnim->GetStatus() == EStatus_Active)
    {
      _elizaAnim->SetCenterPos({animPos.x, 300});

      // 복귀 동작 (Fade Out 으로 사라짐)
      if (elapsedTime >= stopTime)
      {
        if (fadeflag)
        {
          _elizaAnim->FadeOut(1.0f);
          fadeflag = false;
        }
      }
      // 이동 & 대기 단계
      else if (animPos.x < GoalPos.x)
      {
        // 1단계: 전진 이동
        _elizaAnim->SetOpacity("Eliza_Initiative_Gunfire", 1.0f);
        animPos.x += dt * animSpeed;
        if (animPos.x > GoalPos.x)
          animPos.x = GoalPos.x;
        elapsedTime = 0; // 이동 시 타이머 초기화
      }
      else
      {
        // 2단계: 대기 시간 누적
        elapsedTime += dt;
      }
    }
  }
 


  if (_map)
  {
    bool flag = _map->IsGameFinished();
    if (!flag)
    {
      ShowUI(L"PlayBtn");
      HideUI(L"RetryBtn");
      HideUI(L"ApplyBtn");
      _playBtn->_bPlayflag = false;
      //// Eliza Anim 초기화
      if (!_map->isActionTriggered)
      {
        fadeflag = true;
        animPos = DefaultPos;
        elapsedTime = 0.0f;
      }

    }
    else
    {
      HideUI(L"PlayBtn");
      ShowUI(L"RetryBtn");
      ShowUI(L"ApplyBtn");
    }
  }

  // Lv.5, Lv.7, Lv.8_2(9) 제외 GunfireBtn 비활성화
  if (levelIdx == 5 || levelIdx == 6 || levelIdx == 9 || levelIdx == 12 ||
      levelIdx == 13 || levelIdx == 14 || levelIdx == 15 || levelIdx == 16)
  {
    return;
  }
  else
  {
    _gunfireBtn->Deactivate();
  }

  // AudioDramaUI, ResultDialogUI
  for (auto& [name, UIPanel] : _world->_canvas->panelMap)
  {
    if (name == L"AudioDramaUI")
    {
      if (UIPanel->GetStatus() == EStatus_Active)
      {
        this->Deactivate();
        _map->StopBackgroundAudio();
      }
    }
  }

  if (_status == EStatus_Active)
  {
    SetBGM();
  }
}

void InGameUI::SetBGM()
{

  if (prevLevelIdx == levelIdx)
  {
    return;
  }

  if (levelIdx == 1 || levelIdx == 3 || levelIdx == 10 || levelIdx == 13 ||
      levelIdx == 15)
  {
      _map->PlayBackgroundAudio(kBar);
  }
  else if (levelIdx == 2 || levelIdx == 5 || levelIdx == 7 || levelIdx == 8 ||
              levelIdx == 9 || levelIdx == 12 || levelIdx == 14)
  {
      _map->PlayBackgroundAudio(kMuseum);
  }
  else if (levelIdx == 4 || levelIdx == 6 || levelIdx == 11 || levelIdx == 16)
  {
      _map->PlayBackgroundAudio(kWarehouse);
  }

  prevLevelIdx++;
}
