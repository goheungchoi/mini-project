#include "GameApp.h"
#include "../../Engine/Source/WindowManager/WindowManager.h"
void GameApp::Initialize()
{
  // 윈도우 생성
  _hwnd = WindowManager::GetInstance()->CreateWinApp(1920, 1080, L"MiniProject");

  Super::Initialize();
  _renderer.Init_Win32(1920, 1080, nullptr, &_hwnd);
  
}

void GameApp::Execute()
{

  Super::Execute();

}

void GameApp::Shutdown()
{
  _renderer.Cleanup();
  Super::Shutdown();
  
  // 윈도우 파괴
  
}

void GameApp::FixedUpdate(float deltaTime)
{
  
}

void GameApp::Update(float deltaTime)
{

}

void GameApp::Render()
{
  _renderer.BeginFrame();
  _renderer.BeginDraw();
  _renderer.BindPipeline();
  _renderer.EndDraw();
  _renderer.BeginFrame();
}


