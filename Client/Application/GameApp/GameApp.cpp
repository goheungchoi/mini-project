#include "GameApp.h"
#include "WindowManager/WindowManager.h"

void GameApp::Initialize()
{
  // 윈도우 생성
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
  
  // 윈도우 파괴
}

void GameApp::FixedUpdate(float deltaTime)
{
}

void GameApp::Update(float deltaTime)
{
}

void GameApp::Render() {}


