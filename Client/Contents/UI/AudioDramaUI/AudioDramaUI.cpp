#include "AudioDramaUI.h"

#include "Contents/GameObjects/GameManager/GameManager.h"
#include "Contents/SoundList/SoundList.h"
#include "Contents/UI/InGameUI/InGameUI.h"
#include "Contents/UI/MainMenuUI/MainMenuUI.h"
#include "GameFramework/UI/Canvas/Canvas.h"
#include "GameFramework/UI/UIImage/UIImage.h"
#include "GameFramework/UI/UIText/UIText.h"
#include "Contents/GameObjects/Map/Map.h"
#include "Shared/Config/Config.h"
#include "SoundSystem/SoundManager.h"

AudioDramaUI::AudioDramaUI(class World* world):UIPanel(world)
{
  _map = _world->FindGameObjectByType<Map>();
  stageidx = static_cast<GameLevel*>(_world->_currentLevel)->GetStageIdx();
  _Background = CreateUI<UIImage>(L"AudioBackground");

  _Background->SetSprite("2D\\UI\\FadeBlack.png");
  _Background->SetScale({120, 120});
  _Background->SetOpacity(1.f);
  _Background->SetCenterPos({960, 540});

  _AudioScript = CreateUI<UIText>(L"AudioScript");
  _AudioScript->SetSize({1920, 500});
  _AudioScript->SetCenterPos({960, 540});
  _AudioScript->SetFont(L"PT Noeul");
  _AudioScript->SetFontSize(40.f);
  _AudioScript->SetOpacity(0.f);
  _AudioScript->SetColor(Color(1, 1, 1, 1));
  _AudioScript->SetTextAlignment(TextAlignment::CENTERAlIGN);
  _AudioScript->SetParagraphAlignment(ParagraphAlignment::MIDALIGN);
  SetOnActivatedEvent([=]() {
    _elapsedTimer = 0;
    _Background->Activate();
    _Background->FadeIn(2.f);
    _AudioScript->Activate();

    _world->_canvas->GetPanel<InGameUI>(L"InGameUI")->Deactivate();
  });
  SetOnDeactivatedEvent([=]() {
    _Background->FadeOut(1.f);
    //_world->_canvas->GetPanel<InGameUI>(L"InGameUI")->Activate();
    if (isEnding1 || isEnding2)
    {
      _world->PrepareChangeLevel("Main Menu");
      _world->CommitLevelChange();
    }
    else
    {
      _map->ResumeGame();
    }
  });

  _scripts.resize(5);
  _scripts[0].push_back({L"1970년 2월 13일 칼트,\n적국 선전 방송 "});
  _scripts[0].push_back({L"오늘 우리는 알렌의 반란에 대해 이야기하겠습니다.	"});
  _scripts[0].push_back({L"뻔한 결과였죠, 반란은 실패로 끝났고, 결국 칼트가 알렌을 지배했습니다."});
  _scripts[0].push_back({L"무의미한 민간인 피해도 상당했어요."});
  _scripts[0].push_back({L"그런데 말입니다, 만약 그 협정이 저지되었다면?"});
  _scripts[0].push_back({L"글쎄요, 가능성은 극히 낮습니다만,"});
  _scripts[0].push_back({L"그들이 외치던 '혁명'이 되었을지도 모르겠군요."});
  _scripts[0].push_back({L"이번에도… 결국 반란으로 기록됐군."});
  _scripts[0].push_back({L"다시 1940년으로 돌아간다."});
  _scripts[0].push_back({L"이번에는 반드시, 희생 없는 진정한 혁명을 이루겠다.	"});

  _scripts[1].push_back({L" 1970년 2월 13일 칼트,\n 적국 선전 방송"});
  _scripts[1].push_back({L"알렌 레지스탕스의 반란에 대해 이야기하겠습니다."});
  _scripts[1].push_back({L"흠, 반란치곤 꽤 정교하더군요. 끈질기기도 했고."});
  _scripts[1].push_back({L"희생을 최소화하며 목표를 노렸습니다."});
  _scripts[1].push_back({L"특히 협정 습격 사건은 칼트 입장에서 위협적이었죠."});
  _scripts[1].push_back({L"하지만 뭐, 결과는 뻔했잖습니까?"});
  _scripts[1].push_back({L"결국 알렌은 칼트의 질서 아래 무릎을 꿇었죠.	"});
  _scripts[1].push_back({L"뻔한 결과라고?"});
  _scripts[1].push_back({L"내가 뒤집어주지."});

  _scripts[2].push_back({L" 1970년 2월 13일 칼트,\n 적국 선전 방송"});
  _scripts[2].push_back({L"알렌 반란이 가장 위험했던 순간을 돌아보겠습니다.	"});
  _scripts[2].push_back({L"협정 습격 사건, 그때가 저항이 극에 달했었죠."});
  _scripts[2].push_back({L"맞습니다. 그래서 우리도 총력을 다해서 결국 진압했죠.	"});
  _scripts[2].push_back({L"그 혼란 속에서 한 자가 역사 한가운데 섰습니다."});
  _scripts[2].push_back({L"알렌 놈들은 아직도 그 자를 영웅이라 떠받들더군요."});
  _scripts[2].push_back({L"네, 그의 이름은…"});
  _scripts[2].push_back({L"거의 다왔다."});
  _scripts[2].push_back({L"칼트의 시대는 끝난다."});

  _scripts[3].push_back({L" 1970년 2월 13일 칼트,\n 적국 선전 방송"});
  _scripts[3].push_back({L"오늘은 알렌 혁명을 결정지은 순간을 돌아보겠습니다.	"});
  _scripts[3].push_back({L"협정 저지 작전, 바로 이때 알렌의 운명이 바뀌었죠.	"});
  _scripts[3].push_back({L"그 덕에 알렌은 칼트의 지배에서 벗어났습니다.	"});
  _scripts[3].push_back({L"말 그대로, 기적 같은 혁명이었죠.	"});
  _scripts[3].push_back({L"그리고 그 중심에 한 사람이 있었습니다.	"});
  _scripts[3].push_back({L"레지스탕스의 영웅, 엘리자.	"});
  _scripts[3].push_back({L"대장과 함께 전장을 지휘하며 알렌을 승리로 이끌었고,	"});
  _scripts[3].push_back({L"반란이라 기록될 역사를 혁명으로 바꿨습니다.	"});
  _scripts[3].push_back({L"마침내,	"});
  _scripts[3].push_back({L"해냈다.	"});

  _scripts[4].push_back({L" 1970년 2월 13일 칼트,\n 적국 선전 방송"});
  _scripts[4].push_back({L"오늘은 알렌 혁명을 결정지은 순간을 돌아보겠습니다.	"});
  _scripts[4].push_back({L"협정 저지 작전, 바로 이때 알렌의 운명이 바뀌었죠.	"});
  _scripts[4].push_back({L"그 덕에 알렌은 칼트의 지배에서 벗어났습니다.	"});
  _scripts[4].push_back({L"무의미한 희생 없이 이루어진, 기적 같은 혁명이었죠.	"});
  _scripts[4].push_back({L"그리고 그 중심에 그들이 있었습니다.	"});
  _scripts[4].push_back({L"알렌의 영웅, 레지스탕스.	"});
  _scripts[4].push_back({L"대장과 함께 전장을 지휘하며 알렌을 승리로 이끌었고,	"});
  _scripts[4].push_back({L"반란이라 기록될 역사를 혁명으로 바꿨습니다.	"});
  _scripts[4].push_back({L"마침내,	"});
  _scripts[4].push_back({L"해냈다.	"});


  
}

void AudioDramaUI::BeginLevel()
{
  UIPanel::BeginLevel();

}

void AudioDramaUI::Update(float dt)
{
  UIPanel::Update(dt);
  if (PlayFlag)
  {
    _Background->SetOpacity(1.f);
    RunStage(dt);
    if (INPUT.IsKeyPress(Key::Escape))
      Skip();

    if (_elapsedTimer >= _totalTime)
    {
      _Background->FadeOut(1.f);
    }

    if (_elapsedTimer >= _totalTime + 1.f)
    {
      PlayFlag = false;
    _world->_canvas->GetPanel<InGameUI>(L"InGameUI")->Activate();
      if (isEnding1 || isEnding2)
      {
        GameManager::endingflag = true;
        _world->PrepareChangeLevel("Main Menu");
        _world->CommitLevelChange();
      }
      else
        Deactivate();
    }
  }
    //_world->_canvas->GetPanel<InGameUI>(L"InGameUI")->Activate();
    //_Background->SetOpacity(0.f);


}

void AudioDramaUI::Render()
{
  UIPanel::Render();
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
      _AudioScript->Fade(0.2f, 0.f, 1.f);
      _scripts[stageidx][scriptidx].fadein = false;
    }
  }
  else
  {
    if (_scripts[stageidx][scriptidx].fadeout)
    {
      _AudioScript->Fade(0.2f, 1.f, 0.f);
      _scripts[stageidx][scriptidx].fadeout = false;
    }   
  }

}


void AudioDramaUI::Stage1(float dt)
{
  isEnding1 = false;
  isEnding2 = false;
  _elapsedTimer += dt;
  if (_elapsedTimer >=0)     PrintText(0, 0, dt, 2);
  if (_elapsedTimer >=2.11)  PrintText(0, 1, dt, 2.98);
  if (_elapsedTimer >=5.23)  PrintText(0, 2, dt, 5.97);
  if (_elapsedTimer >=12.04) PrintText(0, 3, dt, 2.04);
  if (_elapsedTimer >=14.19) PrintText(0, 4, dt, 3.01);
  if (_elapsedTimer >=18.04) PrintText(0, 5, dt, 1.19);
  if (_elapsedTimer >=20.06) PrintText(0, 6, dt, 3.17);
  if (_elapsedTimer >=25.11) PrintText(0, 7, dt, 3.09);
  if (_elapsedTimer >=29.06) PrintText(0, 8, dt, 3.02);
  if (_elapsedTimer >=32.21) PrintText(0, 9, dt, 4.92);
}
void AudioDramaUI::Stage4(float dt)
{
  isEnding1 = false;
  isEnding2 = false;
  _elapsedTimer += dt;
  if (_elapsedTimer >=0    ) PrintText(1, 0, dt, 2.27);
  if (_elapsedTimer >=3.24 ) PrintText(1, 1, dt, 3.89);
  if (_elapsedTimer >=8.02 ) PrintText(1, 2, dt, 5.09);
  if (_elapsedTimer >=14.02) PrintText(1, 3, dt, 2.26);
  if (_elapsedTimer >=17.21) PrintText(1, 4, dt, 4.91);
  if (_elapsedTimer >=23.02) PrintText(1, 5, dt, 3.19);
  if (_elapsedTimer >=27.07) PrintText(1, 6, dt, 3.94);
  if (_elapsedTimer >=33.01) PrintText(1, 7, dt, 1.24);
  if (_elapsedTimer >=35.09) PrintText(1, 8, dt, 2.05);
}
void AudioDramaUI::Stage7(float dt)
{
  isEnding1 = false;
  isEnding2 = false;
  _elapsedTimer += dt;
  if (_elapsedTimer >=0    ) PrintText(2, 0, dt, 2   );
  if (_elapsedTimer >=3    ) PrintText(2, 1, dt, 4.24);
  if (_elapsedTimer >=8.17 ) PrintText(2, 2, dt, 3.97);
  if (_elapsedTimer >=13.03) PrintText(2, 3, dt, 3.22);
  if (_elapsedTimer >=17.11) PrintText(2, 4, dt, 4.04);
  if (_elapsedTimer >=22.11) PrintText(2, 5, dt, 3.99);
  if (_elapsedTimer >=26.25) PrintText(2, 6, dt, 2.02);
  if (_elapsedTimer >=30.06) PrintText(2, 7, dt, 0.99);
  if (_elapsedTimer >=31.18) PrintText(2, 8, dt, 3.03);
}
void AudioDramaUI::EndingStage1(float dt)
{
  isEnding1 = true;
  isEnding2 = false;
  _elapsedTimer += dt;
  if (_elapsedTimer >=0) PrintText(    3, 0,  dt, 2);
  if (_elapsedTimer >=3) PrintText(    3, 1,  dt, 5.26);
  if (_elapsedTimer >=9.22) PrintText( 3, 2,  dt, 4.78);
  if (_elapsedTimer >=14.2) PrintText( 3, 3,  dt, 4.03);
  if (_elapsedTimer >=19.14) PrintText(3, 4,  dt, 2.99);
  if (_elapsedTimer >=23.06) PrintText(3, 5,  dt, 2.15);
  if (_elapsedTimer >=26.09) PrintText(3, 6,  dt, 2.95);
  if (_elapsedTimer >=29.17) PrintText(3, 7,  dt, 4.93);
  if (_elapsedTimer >=35.01) PrintText(3, 8,  dt, 2.28);
  if (_elapsedTimer >=39.05) PrintText(3, 9,  dt, 0.97);
  if (_elapsedTimer >=40.25) PrintText(3, 10, dt, 1.98);

}
void AudioDramaUI::EndingStage2(float dt)
{
  isEnding1 = false;
  isEnding2 = true;
  _elapsedTimer += dt;
  if (_elapsedTimer >=0) PrintText(4, 0,  dt, 2);
  if (_elapsedTimer >=3) PrintText(4, 1,  dt, 5.26);
  if (_elapsedTimer >=9.22) PrintText(4, 2,  dt, 4.78);
  if (_elapsedTimer >=14.2) PrintText(4, 3,  dt, 4.03);
  if (_elapsedTimer >=19.14) PrintText(4, 4,  dt, 3.89);
  if (_elapsedTimer >=23.21) PrintText(4, 5,  dt, 2.02);
  if (_elapsedTimer >=26.15) PrintText(4, 6,  dt, 2.88);
  if (_elapsedTimer >=29.25) PrintText(4, 7,  dt, 4.79);
  if (_elapsedTimer >=35.01) PrintText(4, 8,  dt, 2.28);
  if (_elapsedTimer >=39.03) PrintText(4, 9,  dt, 0.99);
  if (_elapsedTimer >=41) PrintText(4, 10, dt, 1.23);
  }

void AudioDramaUI::RunStage(float dt)
{
  switch (stageidx)
  {
  case 1:
    _totalTime = 37.f;
    if (_elapsedTimer <= 0.f)
      SoundManager::PlaySound(SoundList::AudioDrama_Stage_01);
    _currentAudio = SoundList::AudioDrama_Stage_01;
    Stage1(dt);
    break;
  case 4:
    _totalTime = 37.f;
    if (_elapsedTimer <= 0.f)
      SoundManager::PlaySound(SoundList::AudioDrama_Stage_04);
    _currentAudio = SoundList::AudioDrama_Stage_04;
    Stage4(dt);
    break;
  case 7:
    _totalTime = 34.f;
    if (_elapsedTimer <= 0.f)
      SoundManager::PlaySound(SoundList::AudioDrama_Stage_07);
    _currentAudio = SoundList::AudioDrama_Stage_07;
    Stage7(dt);
    break;

  case 8:
    _totalTime = 49.f;
    if (_elapsedTimer <= 0.f)
      SoundManager::PlaySound(SoundList::AudioDrama_BadEnding);
    _currentAudio = SoundList::AudioDrama_BadEnding;
    EndingStage1(dt);
    break;

  case 9:
    _totalTime = 49.f;
    if (_elapsedTimer <= 0.f)
      SoundManager::PlaySound(SoundList::AudioDrama_GoodEnding);
    _currentAudio = SoundList::AudioDrama_GoodEnding;
    EndingStage2(dt);
    break;

  }
}

void AudioDramaUI::Skip()
{
  SoundManager::StopSound(_currentAudio);
  _AudioScript->Deactivate();
  _elapsedTimer = _totalTime;
}



