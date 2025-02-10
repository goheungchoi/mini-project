#include "AudioDramaUI.h"

#include "Contents/SoundList/SoundList.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"
#include "Shared/Config/Config.h"
#include "SoundSystem/SoundManager.h"

AudioDramaUI::AudioDramaUI(class World* world):UIPanel(world)
{
  _Background = CreateUI<UIImage>(L"AudioBackground");

  _Background->SetSprite("2D\\UI\\FadeBlack.png");
  _Background->SetScale({120, 120});
  _Background->SetOpacity(1.f);
  _Background->SetCenterPos({960, 540});


  _AudioScript = CreateUI<UIText>(L"AudioScript");
  _AudioScript->SetSize({1920, 500});
  _AudioScript->SetCenterPos({960, 540});
  _AudioScript->SetFont(L"PT Noeul");
  _AudioScript->SetFontSize(35.f);
  _AudioScript->SetOpacity(0.f);
  _AudioScript->SetColor(Color(1, 1, 1, 1));
  _AudioScript->SetTextAlignment(TextAlignment::CENTERAlIGN);
  _AudioScript->SetParagraphAlignment(ParagraphAlignment::MIDALIGN);

  OnActivated = [=]() { _elapsedTimer = 0; };
  OnDeactivated = [=]() {
    if (isEnding1 || isEnding2)
    {
      world->PrepareChangeLevel("Main Menu");
      world->CommitLevelChange();
    }
  };
  _scripts.resize(5);
  _scripts[0].push_back({L"명령 떨어지기 전까지 멋대로 움직이지 마, 제이미."});
  _scripts[0].push_back({L"대장이 신호를 하면,놈들의 머리통을 날려."});
  _scripts[0].push_back({L"네... 하지만 루크 선배, 제가 잘 할 수 있을까요?"});
  _scripts[0].push_back({L"고민만 하다간 죽는다 꼬마야.배운 대로 움직여."});

  _scripts[1].push_back({L"명령 떨어지기 전까지 멋대로 움직이지 마, 제이미."});
  _scripts[1].push_back({L"대장이 신호를 하면,놈들의 머리통을 날려."});
  _scripts[1].push_back({L"네... 하지만 루크 선배, 제가 잘 할 수 있을까요?"});
  _scripts[1].push_back({L"고민만 하다간 죽는다 꼬마야.배운 대로 움직여."});

  _scripts[2].push_back({L"명령 떨어지기 전까지 멋대로 움직이지 마, 제이미."});
  _scripts[2].push_back({L"대장이 신호를 하면,놈들의 머리통을 날려."});
  _scripts[2].push_back({L"네... 하지만 루크 선배, 제가 잘 할 수 있을까요?"});
  _scripts[2].push_back({L"고민만 하다간 죽는다 꼬마야.배운 대로 움직여."});

  _scripts[3].push_back({L"명령 떨어지기 전까지 멋대로 움직이지 마, 제이미."});
  _scripts[3].push_back({L"대장이 신호를 하면,놈들의 머리통을 날려."});
  _scripts[3].push_back({L"네... 하지만 루크 선배, 제가 잘 할 수 있을까요?"});
  _scripts[3].push_back({L"고민만 하다간 죽는다 꼬마야.배운 대로 움직여."});

  _scripts[4].push_back({L"명령 떨어지기 전까지 멋대로 움직이지 마, 제이미."});
  _scripts[4].push_back({L"대장이 신호를 하면,놈들의 머리통을 날려."});
  _scripts[4].push_back({L"네... 하지만 루크 선배, 제가 잘 할 수 있을까요?"});
  _scripts[4].push_back({L"고민만 하다간 죽는다 꼬마야.배운 대로 움직여."});


  
}

void AudioDramaUI::Update(float dt)
{
  UIPanel::Update(dt);

  RunStage(dt);
  if (INPUT.IsKeyPress(MouseState::LB))
    Skip();

  if (_elapsedTimer>= _totalTime)
  {
    _Background->FadeOut(1.f);
  }

  if (_elapsedTimer>= _totalTime+1.f)
    Deactivate();

}

void AudioDramaUI::PrintText(int stageidx, int scriptidx,float dt,
                             float duration)
{

  _scripts[stageidx][scriptidx].time += dt;

  if (_scripts[stageidx][scriptidx].time <= duration)
  {
    _AudioScript->SetText(_scripts[stageidx][scriptidx].script.c_str());
    if (_scripts[stageidx][scriptidx].fadein)
    {
      _AudioScript->Fade(1.f, 0.f, 1.f);
      _scripts[stageidx][scriptidx].fadein = false;
    }
  }
  else
  {
    if (_scripts[stageidx][scriptidx].fadeout)
    {
      _AudioScript->Fade(1.f, 1.f, 0.f);
      _scripts[stageidx][scriptidx].fadeout = false;
    }   
  }

}


void AudioDramaUI::Stage1(float dt)
{
  isEnding1 = false;
  isEnding2 = false;
  _elapsedTimer += dt;
  if (_elapsedTimer >= 4.19)
    PrintText(0, 0, dt, 3.96);
  if (_elapsedTimer >= 9.16)
    PrintText(0, 1, dt, 3.99);
  if (_elapsedTimer >= 14.23)
    PrintText(0, 2, dt, 3.82);
  if (_elapsedTimer >= 19.08)
    PrintText(0, 3, dt, 5.17);
}
void AudioDramaUI::Stage4(float dt)
{
  isEnding1 = false;
  isEnding2 = false;
  _elapsedTimer += dt;
  if (_elapsedTimer >= 4.19)
    PrintText(1, 0, dt, 3.96);
  if (_elapsedTimer >= 9.16)
    PrintText(1, 1, dt, 3.99);
  if (_elapsedTimer >= 14.23)
    PrintText(1, 2, dt, 3.82);
  if (_elapsedTimer >= 19.08)
    PrintText(1, 3, dt, 5.17);
}
void AudioDramaUI::Stage7(float dt)
{
  isEnding1 = false;
  isEnding2 = false;
  _elapsedTimer += dt;
  if (_elapsedTimer >= 4.19)
    PrintText(2, 0, dt, 3.96);
  if (_elapsedTimer >= 9.16)
    PrintText(2, 1, dt, 3.99);
  if (_elapsedTimer >= 14.23)
    PrintText(2, 2, dt, 3.82);
  if (_elapsedTimer >= 19.08)
    PrintText(2, 3, dt, 5.17);
}
void AudioDramaUI::EndingStage1(float dt)
{
  isEnding1 = true;
  isEnding2 = false;
  _elapsedTimer += dt;
  if (_elapsedTimer >= 4.19)
    PrintText(3, 0, dt, 3.96);
  if (_elapsedTimer >= 9.16)
    PrintText(3, 1, dt, 3.99);
  if (_elapsedTimer >= 14.23)
    PrintText(3, 2, dt, 3.82);
  if (_elapsedTimer >= 19.08)
    PrintText(3, 3, dt, 5.17);
}

void AudioDramaUI::EndingStage2(float dt)
{
  isEnding1 = false;
  isEnding2 = true;
  _elapsedTimer += dt;
  if (_elapsedTimer >= 4.19)    PrintText(4, 0, dt, 3.96);
  if (_elapsedTimer >= 9.16)    PrintText(4, 1, dt, 3.99);
  if (_elapsedTimer >= 14.23)   PrintText(4, 2, dt, 3.82);
  if (_elapsedTimer >= 19.08)   PrintText(4, 3, dt, 5.17);
}

void AudioDramaUI::RunStage(float dt)
{
  switch (stageidx)
  {
  case 1:
    if (_elapsedTimer <= 0.f)
      SoundManager::PlaySound(SoundList::AudioDrama_Stage_01);
    _currentAudio = SoundList::AudioDrama_Stage_01;
    Stage1(dt);
    break;
  case 4:
    if (_elapsedTimer <= 0.f)
      SoundManager::PlaySound(SoundList::AudioDrama_Stage_01);
    _currentAudio = SoundList::AudioDrama_Stage_01;
    Stage4(dt);
    break;
  case 7:
    if (_elapsedTimer <= 0.f)
      SoundManager::PlaySound(SoundList::AudioDrama_Stage_01);
    _currentAudio = SoundList::AudioDrama_Stage_01;
    Stage7(dt);
    break;

  case 8:
    if (_elapsedTimer <= 0.f)
      SoundManager::PlaySound(SoundList::AudioDrama_Stage_01);
    _currentAudio = SoundList::AudioDrama_Stage_01;
    EndingStage1(dt);
    break;

  case 9:
    if (_elapsedTimer <= 0.f)
      SoundManager::PlaySound(SoundList::AudioDrama_Stage_01);
    _currentAudio = SoundList::AudioDrama_Stage_01;
    EndingStage2(dt);
    break;

  }
}

void AudioDramaUI::Skip()
{
  SoundManager::StopSound(_currentAudio);
  _elapsedTimer = _totalTime;
}



