#include "GameEngine/GameEngine.h"
#include "Core/Time/TimeSystem.h"
#include "WindowManager/WindowManager.h"
#include "Core/Input/InputSystem.h"

void GameEngine::Initialize()
{
    // 윈도우 크기 조정 이벤트 관련 함수들 설정
  WindowManager::GetInstance()->SetWindowEventCallbacks(_hwnd,
      [this]() -> bool { return this->OnActivated(); },
      [this]() -> bool { return this->OnDeactivated(); },
      [this]() -> bool { return this->OnSuspending(); },
      [this]() -> bool { return this->OnResuming(); },
      [this]() -> bool { return this->OnWindowResized(); });

  // Com 라이브러리 초기화
  HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

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

bool GameEngine::OnActivated()
{
  // TODO: 게임이 활성화된 윈도우가 됩니다.
  std::cout << "GameEngine::OnActivated()" << std::endl;

  return true;
}

bool GameEngine::OnDeactivated()
{
  // TODO: 게임이 백그라운드 윈도우로 전환됩니다.
  std::cout << "GameEngine::OnDeactivated()" << std::endl;
  
  return true;
}

bool GameEngine::OnSuspending()
{
  // TODO: 게임이 절전 모드로 전환됩니다(또는 최소화됩니다).
  std::cout << "GameEngine::OnSuspending()" << std::endl;

  return true;
}

bool GameEngine::OnResuming()
{
  // TODO: 게임이 절전 모드에서 복귀(또는 최소화 복귀)합니다.
  std::cout << "GameEngine::OnResuming()" << std::endl;

  return true;
}

bool GameEngine::OnWindowResized()
{
  // TODO: 게임 윈도우 크기가 조정됩니다.
  std::cout << "GameEngine::OnWindowResized()" << std::endl;

  return true;
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
