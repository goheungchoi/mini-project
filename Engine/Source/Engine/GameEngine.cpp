#include "GameEngine.h"
#include "Core/Time/TimeSystem.h"

void GameEngine::Initialize()
{
  // TimeSystem �ʱ�ȭ
  TimeSystem::Initialize();

  // InputSystem �ʱ�ȭ
  // Renderer �ʱ�ȭ

  // NOTE: ���� �ʱ�ȭ �۾�
  // e.g., ���ҽ� �Ŵ���, ���丮 �̱��� ��...
}

void GameEngine::Execute()
{
  Run();
}

void GameEngine::Shutdown()
{
  // NOTE: ���� ���� �۾�
  // e.g., �޸� ����, �̱��� �˴ٿ� ��...
}

void GameEngine::Run()
{
  TimeSystem::Reset();

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
      Update(TimeSystem::GetDeltaTime());
      Render();
    }
  }
}
