#include "GameEngine/GameEngine.h"
#include "Core/Time/TimeSystem.h"
#include "WindowManager/WindowManager.h"
#include "Core/Input/InputSystem.h"

void GameEngine::Initialize()
{
    // ������ ũ�� ���� �̺�Ʈ ���� �Լ��� ����
  WindowManager::GetInstance()->SetWindowEventCallbacks(_hwnd,
      [this]() -> bool { return this->OnActivated(); },
      [this]() -> bool { return this->OnDeactivated(); },
      [this]() -> bool { return this->OnSuspending(); },
      [this]() -> bool { return this->OnResuming(); },
      [this]() -> bool { return this->OnWindowResized(); });

  // Com ���̺귯�� �ʱ�ȭ
  HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

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

bool GameEngine::OnActivated()
{
  // TODO: ������ Ȱ��ȭ�� �����찡 �˴ϴ�.
  std::cout << "GameEngine::OnActivated()" << std::endl;

  return true;
}

bool GameEngine::OnDeactivated()
{
  // TODO: ������ ��׶��� ������� ��ȯ�˴ϴ�.
  std::cout << "GameEngine::OnDeactivated()" << std::endl;
  
  return true;
}

bool GameEngine::OnSuspending()
{
  // TODO: ������ ���� ���� ��ȯ�˴ϴ�(�Ǵ� �ּ�ȭ�˴ϴ�).
  std::cout << "GameEngine::OnSuspending()" << std::endl;

  return true;
}

bool GameEngine::OnResuming()
{
  // TODO: ������ ���� ��忡�� ����(�Ǵ� �ּ�ȭ ����)�մϴ�.
  std::cout << "GameEngine::OnResuming()" << std::endl;

  return true;
}

bool GameEngine::OnWindowResized()
{
  // TODO: ���� ������ ũ�Ⱑ �����˴ϴ�.
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
