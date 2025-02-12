#include "GameApp.h"

#include "Core/Input/InputSystem.h"
#include "Core/Time/TimeSystem.h"
#include "ResourceManager/ResourceManager.h"
#include "WindowManager/WindowManager.h"

#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"

#include "Contents/Levels/GameLevel.h"
#include "Contents/Levels/MainMenu/MainMenu.h"
#include "Contents/Levels/Level1/Level1.h"
#include "Contents/Levels/Level2/Level2.h"
#include "Contents/Levels/Level3/Level3.h"
#include "Contents/Levels/Level4/Level4.h"
#include "Contents/Levels/Level5/Level5.h"
#include "Contents/Levels/Level6/Level6.h"
#include "Contents/Levels/Level7/Level7.h"
#include "Contents/Levels/Level8/Level8.h"
#include "Contents/Levels/Level8_2/Level8_2.h"
#include "Contents/Levels/DialogLevel/DialogLevel.h"
#include "Contents/Levels/Level10_1/Level10_1.h"
#include "Contents/Levels/Level10_2/Level10_2.h"
#include "Contents/Levels/Level10_3/Level10_3.h"
#include "Contents/Levels/Level10_4/Level10_4.h"
#include "Contents/Levels/Level10_5/Level10_5.h"
#include "Contents/Levels/Level10_6/Level10_6.h"
#include "Contents/Levels/Level10_7/Level10_7.h"
#include "Contents/Levels/ChallengeMode/ChallengeMode.h"
#include "Contents/SoundList/SoundList.h"

static GameLevel* gameLevel;
static MainMenu* mainMenuLevel;
static Level1* level1;
static Level2* level2;
static Level3* level3;
static Level4* level4;
static Level5* level5;
static Level6* level6;
static Level7* level7;
static Level8* level8;
static Level8_2* level8_2;
static Level10_1* level10_1;
static Level10_2* level10_2;
static Level10_3* level10_3;
static Level10_4* level10_4;
static Level10_5* level10_5;
static Level10_6* level10_6;
static Level10_7* level10_7;
static DialogLevel* dialogLevel;
static ChallengeMode* challengeLevel;


// Challenge mode.



void GameApp::Initialize(UINT screenWidth, UINT screenHeight,
                         const std::wstring& title)
{
  Super::Initialize(screenWidth, screenHeight, title);

  // 디버그용 콘솔창 생성 *Debug*
  //bool bUseConsole = true; // 이거 true로 바꾸면 콘솔창 뜸.
  //if (bUseConsole)
  //{
  //  AllocConsole();
  //  FILE* _tempFile;
  //  freopen_s(&_tempFile, "CONOUT$", "w", stdout);
  //}

  RegisterSoundList();

  _world = World::CreateWorld(_hwnd, title);

  gameLevel = new GameLevel("Game Level");
  mainMenuLevel = new MainMenu("Main Menu");
  dialogLevel = new DialogLevel("Dialog Level");
  challengeLevel = new ChallengeMode("Challenge Level");
  level1 = new Level1("Level1");
  level1->BindDialogLevel(dialogLevel);
  level2 = new Level2("Level2");
  level2->BindDialogLevel(dialogLevel);
  level3 = new Level3("Level3");
  level3->BindDialogLevel(dialogLevel);
  level4 = new Level4("Level4");
  level4->BindDialogLevel(dialogLevel);
  level5 = new Level5("Level5");
  level5->BindDialogLevel(dialogLevel);
  level6 = new Level6("Level6");
  level6->BindDialogLevel(dialogLevel);
  level7 = new Level7("Level7");
  level7->BindDialogLevel(dialogLevel);
  level8 = new Level8("Level8");
  level8->BindDialogLevel(dialogLevel);
  level8_2 = new Level8_2("Level8_2");
  level8_2->BindDialogLevel(dialogLevel);
  level10_1 = new Level10_1("Level10_1");
  level10_1->BindDialogLevel(dialogLevel);
  level10_2 = new Level10_2("Level10_2");
  level10_2->BindDialogLevel(dialogLevel);
  level10_3 = new Level10_3("Level10_3");
  level10_3->BindDialogLevel(dialogLevel);
  level10_4 = new Level10_4("Level10_4");
  level10_4->BindDialogLevel(dialogLevel);
  level10_5 = new Level10_5("Level10_5");
  level10_5->BindDialogLevel(dialogLevel);
  level10_6 = new Level10_6("Level10_6");
  level10_6->BindDialogLevel(dialogLevel);
  level10_7 = new Level10_7("Level10_7");
  level10_7->BindDialogLevel(dialogLevel);
  level1->SetStageIdx(1);
  level2->SetStageIdx(2);
  level3->SetStageIdx(3);
  level4->SetStageIdx(4);
  level5->SetStageIdx(5);
  level6->SetStageIdx(6);
  level7->SetStageIdx(7);
  level8->SetStageIdx(8);
  level8_2->SetStageIdx(9);
  level10_1->SetStageIdx(10);
  level10_2->SetStageIdx(11);
  level10_3->SetStageIdx(12);
  level10_4->SetStageIdx(13);
  level10_5->SetStageIdx(14);
  level10_6->SetStageIdx(15);
  level10_7->SetStageIdx(16);


  _world->AddLevel(gameLevel);
  _world->AddLevel(mainMenuLevel);
  _world->AddLevel(dialogLevel);
  _world->AddLevel(level1);
  _world->AddLevel(level2);
  _world->AddLevel(level3);
  _world->AddLevel(level4);
  _world->AddLevel(level5);
  _world->AddLevel(level6);
  _world->AddLevel(level7);
  _world->AddLevel(level8);
  _world->AddLevel(level8_2);
  _world->AddLevel(challengeLevel);
  _world->AddLevel(level10_1);
  _world->AddLevel(level10_2);
  _world->AddLevel(level10_3);
  _world->AddLevel(level10_4);
  _world->AddLevel(level10_5);
  _world->AddLevel(level10_6);
  _world->AddLevel(level10_7);

  _world->PrepareChangeLevel(mainMenuLevel->name);
  _world->CommitLevelChange();
}

void GameApp::Execute()
{
  Super::Execute();
}

void GameApp::Shutdown()
{   
  delete gameLevel;
  delete mainMenuLevel;
  delete dialogLevel;
  delete challengeLevel;
  delete level1;
  delete level2;
  delete level3;
  delete level4;
  delete level5;
  delete level6;
  delete level7;
  delete level8;
  delete level8_2;
  delete level10_1;
  delete level10_2;
  delete level10_3;
  delete level10_4;
  delete level10_5;
  delete level10_6;
  delete level10_7;
  _world->Shutdown();
  delete _world;
  Super::Shutdown();
}

void GameApp::ProcessInput(float dt)
{
  _world->ProcessInput(dt);
}

void GameApp::FixedUpdate(float fixedRate)
{
}

void GameApp::Update(float dt)
{
  _world->ProcessInput(dt);
  _world->PreUpdate(dt);
  _world->AnimationUpdate(dt);
  _world->Update(dt);
  _world->PostUpdate(dt);
}

void GameApp::Render()
{
  _world->RenderGameObjects();
  _world->RenderUI();
}

bool GameApp::OnActivated()
{
  // TODO: 게임이 활성화된 윈도우가 됩니다.

  std::cout << "GameApp::OnActivated()" << std::endl;

  return true;
}

bool GameApp::OnDeactivated()
{
  // TODO: 게임이 백그라운드 윈도우로 전환됩니다.

  std::cout << "GameApp::OnDeactivated()" << std::endl;

  return true;
}

bool GameApp::OnSuspending()
{
  // TODO: 게임이 절전 모드로 전환됩니다(또는 최소화됩니다).

  std::cout << "GameApp::OnSuspending()" << std::endl;

  return true;
}

bool GameApp::OnResuming()
{
  // TODO: 게임이 절전 모드에서 복귀(또는 최소화 복귀)합니다.

  std::cout << "GameApp::OnResuming()" << std::endl;

  return true;
}

bool GameApp::OnWindowResized()
{
  // TODO: 게임 윈도우 크기가 조정됩니다.

  std::cout << "GameApp::OnWindowResized()" << std::endl;

  return true;
}

void GameApp::Run()
{
  TimeSystem::Reset();

  double dt = 0.0;               // 델타 타임
  double dtThreshole = 0.25;     // 최대 델타 타임
  double fixedRate = kFixedRate; // 픽스 타임
  double accumulator = 0.0;      // 픽스 타임 트레킹

  // Main Loop
  MSG msg = {};
  bool bQuit = false;

  while (!bQuit)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
      {
        bQuit = true;
        break;
      }

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      TimeSystem::Update();

      dt = TimeSystem::GetDeltaTime();

      if (_world->bChangingLevel)
        continue;

      _world->InitialStage();

      // 게임 루프 소용돌이를 방어하기 위해 델타타임이 일정시간 이상 넘어가면
      // 픽스 합니다.
      dtThreshole = 0.25;
      if (dt > dtThreshole)
        dt = dtThreshole; // Clamp the frame time

      // 프레임 타임을 누적해서 픽스 타임을 추적합니다.
      accumulator += dt;
      // 일정 시간마다 픽스 업데이트를 실행합니다.
      fixedRate = kFixedRate * dt; // 스케일 된 픽스 타임
      while (accumulator >= fixedRate)
      {
        FixedUpdate(fixedRate); // Update physics at a fixed rate
        accumulator -= fixedRate;
      }

      Update(dt);
      Render();

      _world->CleanupStage();
    }
  }
}
