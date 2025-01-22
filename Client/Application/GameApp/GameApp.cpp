#include "GameApp.h"
#include "../../Engine/Source/Renderer/DX11/DX11Renderer.h"
#include "Core/Input/InputSystem.h"
#include "ResourceManager/ResourceManager.h"
#include "WindowManager/WindowManager.h"

#include "GameFramework/World/World.h"
#include "GameFramework/Level/Level.h"

#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimationState.h"
#include "GameFramework/Components/Animation/AnimatorComponent.h"

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

  

}

void GameApp::Execute()
{
  Super::Execute();
}

void GameApp::Shutdown()
{

  Super::Shutdown();
}

void GameApp::FixedUpdate(float deltaTime) {

}

void GameApp::Update(float deltaTime)
{

}

void GameApp::Render()
{

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
