#include "GameApp.h"
#include "../../Engine/Source/WindowManager/WindowManager.h"

void GameApp::Initialize()
{
  // ������ ����
  _hwnd = WindowManager::GetInstance()->CreateWinApp();

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


