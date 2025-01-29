#include "GameApp.h"

#include "Core/Input/InputSystem.h"
#include "Core/Time/TimeSystem.h"
#include "ResourceManager/ResourceManager.h"
#include "WindowManager/WindowManager.h"

#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"

#include "Contents/Levels/TestLevel.h"
#include "Contents/Prototype/GameLevel.h"
#include "Contents/Prototype/GridLevel.h"

static GridLevel* gridLevel;
static TestLevel* testLevel;

static GameLevel* gameLevel;

void GameApp::Initialize(UINT screenWidth, UINT screenHeight,
                         const std::wstring& title)
{
  Super::Initialize(screenWidth, screenHeight, title);

  // ����׿� �ܼ�â ���� *Debug*
  bool bUseConsole = true; // �̰� true�� �ٲٸ� �ܼ�â ��.
  if (bUseConsole)
  {
    AllocConsole();
    FILE* _tempFile;
    freopen_s(&_tempFile, "CONOUT$", "w", stdout);
  }

  _world = World::CreateWorld(_hwnd, title);

  gridLevel = new GridLevel("Test Grid Level", 5, 5);
  testLevel = new TestLevel();

  gameLevel = new GameLevel("Test Game Level");

  _world->AddLevel(gridLevel);
  _world->AddLevel(testLevel);
  _world->AddLevel(gameLevel);
  _world->PrepareChangeLevel(gameLevel->name);
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
  //_world->FixedUpdate(fixedRate);
  //_world->PhysicsUpdate(fixedRate);
}

void GameApp::Update(float dt)
{
  _world->ProcessInput(dt);
  _world->PreUpdate(dt);
  _world->Update(dt);
  _world->AnimationUpdate(dt);
  _world->PostUpdate(dt);
}

void GameApp::Render()
{
  _world->RenderGameObjects();
  _world->RenderUI();
}

bool GameApp::OnActivated()
{
  // TODO: ������ Ȱ��ȭ�� �����찡 �˴ϴ�.

  std::cout << "GameApp::OnActivated()" << std::endl;

  return true;
}

bool GameApp::OnDeactivated()
{
  // TODO: ������ ��׶��� ������� ��ȯ�˴ϴ�.

  std::cout << "GameApp::OnDeactivated()" << std::endl;

  return true;
}

bool GameApp::OnSuspending()
{
  // TODO: ������ ���� ���� ��ȯ�˴ϴ�(�Ǵ� �ּ�ȭ�˴ϴ�).

  std::cout << "GameApp::OnSuspending()" << std::endl;

  return true;
}

bool GameApp::OnResuming()
{
  // TODO: ������ ���� ��忡�� ����(�Ǵ� �ּ�ȭ ����)�մϴ�.

  std::cout << "GameApp::OnResuming()" << std::endl;

  return true;
}

bool GameApp::OnWindowResized()
{
  // TODO: ���� ������ ũ�Ⱑ �����˴ϴ�.

  std::cout << "GameApp::OnWindowResized()" << std::endl;

  return true;
}

void GameApp::Run()
{
  TimeSystem::Reset();

  double dt = 0.0;               // ��Ÿ Ÿ��
  double dtThreshole = 0.25;     // �ִ� ��Ÿ Ÿ��
  double fixedRate = kFixedRate; // �Ƚ� Ÿ��
  double accumulator = 0.0;      // �Ƚ� Ÿ�� Ʈ��ŷ

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

      if (_world->changingLevel)
        continue;

      _world->InitialStage();

      // ���� ���� �ҿ뵹�̸� ����ϱ� ���� ��ŸŸ���� �����ð� �̻� �Ѿ��
      // �Ƚ� �մϴ�.
      dtThreshole = 0.25;
      if (dt > dtThreshole)
        dt = dtThreshole; // Clamp the frame time

      // ������ Ÿ���� �����ؼ� �Ƚ� Ÿ���� �����մϴ�.
      accumulator += dt;
      // ���� �ð����� �Ƚ� ������Ʈ�� �����մϴ�.
      fixedRate = kFixedRate * dt; // ������ �� �Ƚ� Ÿ��
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
