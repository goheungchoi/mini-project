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


  // ElizaAnim �� Active�����̸� ��ġ ������
  UIAnim* _elizaAnim = _gunfireBtn->GetUI<UIAnim>(L"ElizaAnim");

 if (_elizaAnim && _elizaAnim->GetStatus() == EStatus_Active)
  {
    _elizaAnim->SetCenterPos({animPos.x, 300});

    // ���� ���� (Fade Out ���� �����)
    if (elapsedTime >= stopTime)
    {
      // 3�ܰ�: ���� �̵�
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
    // �̵� & ��� �ܰ�
    else if (animPos.x < GoalPos.x)
    {
      // 1�ܰ�: ���� �̵�
      animPos.x += dt * animSpeed;
      if (animPos.x > GoalPos.x)
        animPos.x = GoalPos.x;
      elapsedTime = 0; // �̵� �� Ÿ�̸� �ʱ�ȭ
    }
    else
    {
      // 2�ܰ�: ��� �ð� ����
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
