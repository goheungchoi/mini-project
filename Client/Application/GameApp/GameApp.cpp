#include "GameApp.h"
#include "WindowManager/WindowManager.h"

void GameApp::Initialize()
{
  // ������ ����
  _hwnd = WindowManager::GetInstance()->CreateWinApp(1920, 1080, L"MiniProject");

  Super::Initialize();

}

void GameApp::Execute()
{

  Super::Execute();

}

void GameApp::Shutdown()
{
  Super::Shutdown();
  
  // ������ �ı�
}

void GameApp::FixedUpdate(float deltaTime)
{
}

void GameApp::Update(float deltaTime)
{
}

void GameApp::Render() {}


