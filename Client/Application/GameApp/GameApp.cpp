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


void GameApp::Initialize(UINT screenWidth, UINT screenHeight,
                         const std::wstring& title)
{
  Super::Initialize(screenWidth, screenHeight, title);

  // 디버그용 콘솔창 생성 *Debug*
  bool bUseConsole = true; // 이거 true로 바꾸면 콘솔창 뜸.
  if (bUseConsole)
  {
    AllocConsole();
    FILE* _tempFile;
    freopen_s(&_tempFile, "CONOUT$", "w", stdout);
  }

  _world = World::CreateWorld(_hwnd, title);

  gameLevel = new GameLevel("Game Level");
  mainMenuLevel = new MainMenu("Main Menu");
  level1 = new Level1("Level1");
  level2 = new Level2("Level2");
  level3 = new Level3("Level3");
  level4 = new Level4("Level4");
  level5 = new Level5("Level5");
  level6 = new Level6("Level6");
  level7 = new Level7("Level7");
  level8 = new Level8("Level8");

  _world->AddLevel(gameLevel);
  _world->AddLevel(mainMenuLevel);
  _world->AddLevel(level1);
  _world->AddLevel(level2);
  _world->AddLevel(level3);
  _world->AddLevel(level4);
  _world->AddLevel(level5);
  _world->AddLevel(level6);
  _world->AddLevel(level7);
  _world->AddLevel(level8);

  _world->PrepareChangeLevel(level1->name);
  _world->CommitLevelChange();
}

void GameApp::Execute()
{
  Super::Execute();
}

void GameApp::Shutdown()
{

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
