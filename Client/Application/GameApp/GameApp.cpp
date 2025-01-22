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
  
  // ����׿� �ܼ�â ���� *Debug*
  bool bUseConsole = true; // �̰� true�� �ٲٸ� �ܼ�â ��.
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
