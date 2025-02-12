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
  _scripts[0].push_back({L"1970�� 2�� 13�� ĮƮ,\n���� ���� ��� "});
  _scripts[0].push_back({L"���� �츮�� �˷��� �ݶ��� ���� �̾߱��ϰڽ��ϴ�.	"});
  _scripts[0].push_back({L"���� �������, �ݶ��� ���з� ������, �ᱹ ĮƮ�� �˷��� �����߽��ϴ�."});
  _scripts[0].push_back({L"���ǹ��� �ΰ��� ���ص� ����߾��."});
  _scripts[0].push_back({L"�׷��� ���Դϴ�, ���� �� ������ �����Ǿ��ٸ�?"});
  _scripts[0].push_back({L"�۽��, ���ɼ��� ���� �����ϴٸ�,"});
  _scripts[0].push_back({L"�׵��� ��ġ�� '����'�� �Ǿ������� �𸣰ڱ���."});
  _scripts[0].push_back({L"�̹������� �ᱹ �ݶ����� ��ϵƱ�."});
  _scripts[0].push_back({L"�ٽ� 1940������ ���ư���."});
  _scripts[0].push_back({L"�̹����� �ݵ��, ��� ���� ������ ������ �̷�ڴ�.	"});

  _scripts[1].push_back({L" 1970�� 2�� 13�� ĮƮ,\n ���� ���� ���"});
  _scripts[1].push_back({L"�˷� ������������ �ݶ��� ���� �̾߱��ϰڽ��ϴ�."});
  _scripts[1].push_back({L"��, �ݶ�ġ�� �� �����ϴ�����. ������⵵ �߰�."});
  _scripts[1].push_back({L"����� �ּ�ȭ�ϸ� ��ǥ�� ��Ƚ��ϴ�."});
  _scripts[1].push_back({L"Ư�� ���� ���� ����� ĮƮ ���忡�� �������̾���."});
  _scripts[1].push_back({L"������ ��, ����� �����ݽ��ϱ�?"});
  _scripts[1].push_back({L"�ᱹ �˷��� ĮƮ�� ���� �Ʒ� ������ �ݾ���.	"});
  _scripts[1].push_back({L"���� ������?"});
  _scripts[1].push_back({L"���� ����������."});

  _scripts[2].push_back({L" 1970�� 2�� 13�� ĮƮ,\n ���� ���� ���"});
  _scripts[2].push_back({L"�˷� �ݶ��� ���� �����ߴ� ������ ���ƺ��ڽ��ϴ�.	"});
  _scripts[2].push_back({L"���� ���� ���, �׶��� ������ �ؿ� ���߾���."});
  _scripts[2].push_back({L"�½��ϴ�. �׷��� �츮�� �ѷ��� ���ؼ� �ᱹ ��������.	"});
  _scripts[2].push_back({L"�� ȥ�� �ӿ��� �� �ڰ� ���� �Ѱ�� �����ϴ�."});
  _scripts[2].push_back({L"�˷� ����� ������ �� �ڸ� �����̶� ���޵������."});
  _scripts[2].push_back({L"��, ���� �̸�����"});
  _scripts[2].push_back({L"���� �ٿԴ�."});
  _scripts[2].push_back({L"ĮƮ�� �ô�� ������."});

  _scripts[3].push_back({L" 1970�� 2�� 13�� ĮƮ,\n ���� ���� ���"});
  _scripts[3].push_back({L"������ �˷� ������ �������� ������ ���ƺ��ڽ��ϴ�.	"});
  _scripts[3].push_back({L"���� ���� ����, �ٷ� �̶� �˷��� ����� �ٲ����.	"});
  _scripts[3].push_back({L"�� ���� �˷��� ĮƮ�� ���迡�� ������ϴ�.	"});
  _scripts[3].push_back({L"�� �״��, ���� ���� �����̾���.	"});
  _scripts[3].push_back({L"�׸��� �� �߽ɿ� �� ����� �־����ϴ�.	"});
  _scripts[3].push_back({L"������������ ����, ������.	"});
  _scripts[3].push_back({L"����� �Բ� ������ �����ϸ� �˷��� �¸��� �̲�����,	"});
  _scripts[3].push_back({L"�ݶ��̶� ��ϵ� ���縦 �������� �ٲ���ϴ�.	"});
  _scripts[3].push_back({L"��ħ��,	"});
  _scripts[3].push_back({L"�س´�.	"});

  _scripts[4].push_back({L" 1970�� 2�� 13�� ĮƮ,\n ���� ���� ���"});
  _scripts[4].push_back({L"������ �˷� ������ �������� ������ ���ƺ��ڽ��ϴ�.	"});
  _scripts[4].push_back({L"���� ���� ����, �ٷ� �̶� �˷��� ����� �ٲ����.	"});
  _scripts[4].push_back({L"�� ���� �˷��� ĮƮ�� ���迡�� ������ϴ�.	"});
  _scripts[4].push_back({L"���ǹ��� ��� ���� �̷����, ���� ���� �����̾���.	"});
  _scripts[4].push_back({L"�׸��� �� �߽ɿ� �׵��� �־����ϴ�.	"});
  _scripts[4].push_back({L"�˷��� ����, ����������.	"});
  _scripts[4].push_back({L"����� �Բ� ������ �����ϸ� �˷��� �¸��� �̲�����,	"});
  _scripts[4].push_back({L"�ݶ��̶� ��ϵ� ���縦 �������� �ٲ���ϴ�.	"});
  _scripts[4].push_back({L"��ħ��,	"});
  _scripts[4].push_back({L"�س´�.	"});


  
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



