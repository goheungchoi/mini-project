#include "GameEngine/GameEngine.h"
#include "Core/Time/TimeSystem.h"
#include "WindowManager/WindowManager.h"
#include "Core/Input/InputSystem.h"

void GameEngine::Initialize()
{
  // TimeSystem �ʱ�ȭ
  TimeSystem::Initialize();

  // InputSystem �ʱ�ȭ
  InputSystem::GetInstance()->Initialize(_hwnd);
  
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

  // InputSystem �ı�
  InputSystem::GetInstance()->Finalize();

  // ������ �ı�
  WindowManager::GetInstance()->DeleteWinApp();
  WindowManager::GetInstance()->DestroyWindowManager();
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
      TimeSystem::Update();
      InputSystem::GetInstance()->Update(TimeSystem::GetDeltaTime());
      Update(TimeSystem::GetDeltaTime());
      Render();
    }
  }
}
