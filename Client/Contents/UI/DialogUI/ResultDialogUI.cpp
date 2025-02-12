#include "ResultDialogUI.h"

#include "Contents/GameObjects/Map/Map.h"
#include "Contents/Levels/DialogLevel/DialogLevel.h"
#include "Contents/UI/AudioDramaUI/AudioDramaUI.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIAnim//UIAnim.h"
#include "GameFramework/UI/UIText/UIText.h"
#include "GameFramework/UI/UIButton/UIButton.h"
#include "Shared/Config/Config.h"



ResultDialogUI::ResultDialogUI(class World* world) : DialogUI(world)
{
  _map = world->FindGameObjectByType<Map>();
  {
    _PhotoPanel->SetSprite("2D\\UI\\FadeBlack.png");
    _PhotoPanel->SetScale({120,120});
    _PhotoPanel->SetOpacity(.5f);
    _PhotoPanel->SetCenterPos({960, 540});
    ShowUI(_PhotoPanel->GetName());
    ShowUI(_Eliza->GetName());
    ShowUI(_speakerNameText->GetName());
    ShowUI(_speakerClassText->GetName());
    ShowUI(_dialogTextBox->GetName());
    ShowUI(_dialogText->GetName());
    ShowUI(_dialogBtnImage->GetName());
    ShowUI(_dialogButton->GetName());

    _playerSelectBtnImage1->SetSprite("2D\\UI\\UI_Selectbox_03.png");
    _playerSelectBtnImage1->SetScale({1.5f, 1.5f});
    _playerSelectBtnImage1->SetCenterPos({1600, 540});
    _playerSelectBtnImage1->SetOpacity(1.f);
    _playerSelectButton1->SetSize({_playerSelectBtnImage1->GetSize().x * 1.3f,
                                   _playerSelectBtnImage1->GetSize().y});
    _playerSelectButton1->SetCenterPos({1550, 540});

    _playerSelectBtnImage2->SetSprite("2D\\UI\\UI_Selectbox_02.png");
    _playerSelectBtnImage2->SetScale({1.5f, 1.5f});
    _playerSelectBtnImage2->SetCenterPos({1600, 700});
    _playerSelectBtnImage2->SetOpacity(1.f);
    _playerSelectButton2->SetSize({_playerSelectBtnImage2->GetSize().x * 1.3f,
                                   _playerSelectBtnImage2->GetSize().y});
    _playerSelectButton2->SetCenterPos({1550, 700});

    _actionList.clear();
    StageIdx = static_cast<GameLevel*>(_world->GetCurrentLevel())->GetStageIdx();

    _Eliza->SetCurrentAnimSprite("Eliza_Resistance_Sad");
    _playerSelectButton1->ClearHandlers();
    _playerSelectButton2->ClearHandlers();
    _dialogButton->AddOnClickHandler([=]() {
      if (!bPlayerSelection)
      NextStep();
    });
    {
      _playerSelectButton1->AddOnHoveredHandler(
          [=]() { _playerSelectBtnImage1->SetOpacity(0.5f); });
      _playerSelectButton1->AddOnUnHoveredHandler(
          [=]() { _playerSelectBtnImage1->SetOpacity(1.f); });
      _playerSelectButton2->AddOnHoveredHandler(
          [=]() { _playerSelectBtnImage2->SetOpacity(0.5f); });
      _playerSelectButton2->AddOnUnHoveredHandler(
          [=]() { _playerSelectBtnImage2->SetOpacity(1.f); });
    }


    OnActivated = [=]() {
      _playerSelectBtnImage1->SetOpacity(1.f); 
      _playerSelectBtnImage2->SetOpacity(1.f); 
      _playerSelectBtnText1->SetText(L"이대로 명령한다");
      _playerSelectBtnText2->SetText(L"다시 생각한다");
      _playerSelectButton1->AddOnClickHandler([=]() {
        if (StageIdx == 8)
        {
          _world->_canvas->GetPanel<AudioDramaUI>(L"AudioDramaUI")->stageidx =
              8;
          _world->_canvas->GetPanel<InGameUI>(L"InGameUI")->Deactivate();
          _world->_canvas->GetPanel<AudioDramaUI>(L"AudioDramaUI")->Activate();
          Deactivate();
        }
        if (StageIdx == 9)
          {
            _world->_canvas->GetPanel<AudioDramaUI>(L"AudioDramaUI")->stageidx =
                9;
            _world->_canvas->GetPanel<InGameUI>(L"InGameUI")->Deactivate();
            _world->_canvas->GetPanel<AudioDramaUI>(L"AudioDramaUI")
                ->Activate();
            Deactivate();
          }

          else
          {
          static_cast<GameLevel*>(_world->_currentLevel)
              ->SetBattleResult(_prevBattleResult);
          static_cast<GameLevel*>(_world->_currentLevel)->SetStageIdx(StageIdx);
          _world->PrepareChangeLevel("Dialog Level");
          _world->CommitLevelChange();
        }
      });
      _playerSelectButton2->AddOnClickHandler([=]() {
        HideUI(_playerSelectBtnImage1->GetName());
        HideUI(_playerSelectButton1->GetName());
        HideUI(_playerSelectBtnText1->GetName());
        HideUI(_playerSelectBtnImage2->GetName());
        HideUI(_playerSelectButton2->GetName());
        HideUI(_playerSelectBtnText2->GetName());
        _world->_canvas->HidePanel(L"ResultDialogUI");
        _world->_canvas->ShowPanel(L"InGameUI");
        if (_map)
          _map->ResetGame();
      });

      if (bIsElizaDead)
      {
        _Eliza->SetOpacity("Eliza_Resistance_Sad", 0.f);
        _speakerClassText->SetOpacity(0.f);
        _speakerNameText->SetText(_namePlayer);

        switch (_prevBattleResult)
        {
        case eBattleResult::CivilDeadWin:
        case eBattleResult::AllyDeadWin:
        case eBattleResult::BothDeadWin:
          _dialogText->SetText(L"이대로 진행한다면 누군가는 희생해야 "
                               L"한다.\n정말 이대로 작전을 지시할까?");
          break;
        case eBattleResult::AllyDeadLose:
          _dialogText->SetText(
              L"이렇게 많은 대원을 잃을 수는 없다.\n다시 작전을 짜보자.");
          break;
        case eBattleResult::Lose:
          _dialogText->SetText(
              L" 남은 적이 지원을 요청한다면 큰일이다.다시 작전을 짜보자.");
          break;
        }
      }
      else
      {
        _speakerNameText->SetText(_nameEliza);
        switch (_prevBattleResult)
        {
        case eBattleResult::CivilDeadWin:
        case eBattleResult::AllyDeadWin:
        case eBattleResult::BothDeadWin:
          _dialogText->SetText(L"이대로 진행한다면 누군가는 희생해야 합니다.\n정말 이대로 작전을 지시할까요?");
          break;
        case eBattleResult::AllyDeadLose:
          _dialogText->SetText(
              L"이렇게 많은 대원을 잃을 수는 없습니다.\n명령을 재고해주세요.");
          break;
        case eBattleResult::Lose:
          _dialogText->SetText(
              L"적이 한 명이라도 남게 되면 지원을 부를 겁니다.\n한 번에 모두 처치할 수 있게 다시 명령을 내려주세요.");
          break;
        }
      }
      _actionList.clear();
      _actionList.push_back([=]() {
        HideUI(_playerSelectBtnImage1->GetName());
        HideUI(_playerSelectButton1->GetName());
        HideUI(_playerSelectBtnText1->GetName());
        HideUI(_playerSelectBtnImage2->GetName());
        HideUI(_playerSelectButton2->GetName());
        HideUI(_playerSelectBtnText2->GetName());
      });
      _actionList.push_back([=]() {
        ShowUI(L"playerbuttonimage2");
        ShowUI(L"playerbutton2");
        ShowUI(L"playerbuttontext2");

        switch (_prevBattleResult)
        {
        case eBattleResult::CivilDeadWin:
        case eBattleResult::AllyDeadWin:
        case eBattleResult::BothDeadWin:
          ShowUI(L"playerbuttonimage1");
          ShowUI(L"playerbutton1");
          ShowUI(L"playerbuttontext1");
          break;
        case eBattleResult::AllyDeadLose:
          break;
        case eBattleResult::Lose:
          break;
        }
      });
    };
  }
}


