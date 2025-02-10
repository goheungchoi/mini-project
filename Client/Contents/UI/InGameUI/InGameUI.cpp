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

  _mainMission = CreateUI<MainMission>(L"MainMission");
  _subMission = CreateUI<SubMission>(L"SubMission");

  _playBtn = CreateUI<PlayButton>(L"PlayBtn");
  _retryBtn = CreateUI<RetryButton>(L"RetryBtn");
  _applyBtn = CreateUI<ApplyButton>(L"ApplyBtn");

  _gunfireBtn = CreateUI<GunfireButton>(L"GunfireBtn");

  _agentStorage = CreateUI<AgentStorage>(L"AgentStorage");

  _cursor = _world->_canvas->GetPanel<UICursor>(L"Cursor");
  _cursor->SetCursorType(CursorType::DEFAULT);

  _gradientIMG = CreateUI<UIImage>(L"GradientIMG");
  _gradientIMG->SetSprite("2D\\UI\\gradient.png", {1600, 1000});
  _gradientIMG->SetScale({1.0f, 0.3f});
}

InGameUI::~InGameUI() {}

void InGameUI::Update(float dt)
{
  __super::Update(dt);

  if (INPUT.IsKeyDown(Key::F1))
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
        ->SetBattleResult(eBattleResult::Lose);


  // ElizaAnim 이 Active상태이면 위치 움직임
  UIAnim* _elizaAnim = _gunfireBtn->GetUI<UIAnim>(L"ElizaAnim");

 if (_elizaAnim && _elizaAnim->GetStatus() == EStatus_Active)
  {
    _elizaAnim->SetCenterPos({animPos.x, 300});

    // 복귀 동작 (Fade Out 으로 사라짐)
    if (elapsedTime >= stopTime)
    {
      // 3단계: 복귀 이동
      //if (animPos.x > BackPos.x)
      //{
      //  animPos.x -= dt * animSpeed;
      //  if (animPos.x < BackPos.x)
      //    animPos.x = BackPos.x;
      //}
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
  else if (_elizaAnim && _elizaAnim->GetStatus() == EStatus_Inactive)
  {
    animPos = DefaultPos;
    elapsedTime = 0.0f;
  }

  if (_map)
  {
    if (!_map->IsGameFinished())
    {
      ShowUI(L"PlayBtn");
      HideUI(L"RetryBtn");
      HideUI(L"ApplyBtn");
    }
    else
    {
      HideUI(L"PlayBtn");
      ShowUI(L"RetryBtn");
      ShowUI(L"ApplyBtn");
    }
  }

}
