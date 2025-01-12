#include "GameEngine/GameEngine.h"
#include "Core/Time/TimeSystem.h"
#include "WindowManager/WindowManager.h"
#include "Core/Input/InputSystem.h"

void GameEngine::Initialize()
{
  // TimeSystem 초기화
  TimeSystem::Initialize();

  // InputSystem 초기화
  InputSystem::GetInstance()->Initialize(_hwnd);
  
  // Renderer 초기화

  // NOTE: 엔진 초기화 작업
  // e.g., 리소스 매니저, 팩토리 싱글톤 등...

}

void GameEngine::Execute()
{
  Run();
}

void GameEngine::Shutdown()
{
  // NOTE: 엔진 종료 작업
  // e.g., 메모리 삭제, 싱글톤 셧다운 등...

  // InputSystem 파괴
  InputSystem::GetInstance()->Finalize();

  // 윈도우 파괴
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
