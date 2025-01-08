#include "GameApp.h"
#include "../../Engine/Source/Renderer/DX11/DX11Renderer.h"
#include "WindowManager/WindowManager.h"
#include "ResourceManager/ResourceManager.h"

#define RenderTest

static ModelHandle modelHandle;
void GameApp::Initialize()
{
  // 윈도우 생성
  _hwnd =
      WindowManager::GetInstance()->CreateWinApp(1920, 1080, L"MiniProject");
  _renderer = new DX11Renderer;
  Super::Initialize();
  _renderer->Init_Win32(1920, 1080, nullptr, &_hwnd);
#ifdef RenderTest
#endif // RenderTest
  modelHandle = LoadModel("Models\\Ceberus\\Ceberus.glb");
  ModelData modelData = AccessModelData(modelHandle);

  std::ranges::for_each(modelData.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
  });
  std::ranges::for_each(modelData.meshes, [&](MeshHandle meshHandle) {
    _renderer->AddRenderPass(meshHandle, RenderPassType::TransparentPass);
  });

  _mainLight.direction = Vector4(1.f, 0.f, 0.f, 0.f);
  _mainLight.color = Vector4(1.f, 1.f, 1.f, 0.f);
  _mainLight.intensity = Vector4(1.f, 1.f, 1.f, 0.f);
}

void GameApp::Execute()
{
  Super::Execute();
}

void GameApp::Shutdown()
{
  _renderer->CreatePipeline();
  _renderer->Cleanup();
  delete _renderer;
  Super::Shutdown();
}

void GameApp::FixedUpdate(float deltaTime) {}

void GameApp::Update(float deltaTime) {}

void GameApp::Render()
{
  Vector4 eye(-100.0f, 0.0f, 0.f, 1.f);
  Matrix view = DirectX::XMMatrixLookAtLH(eye, Vector3::Zero, Vector3::Up);
  Matrix projection = DirectX::XMMatrixPerspectiveFovLH(
      DirectX::XM_PIDIV2, 1920.0f / 1080.0f, 0.01f, 1000.0f);
  _renderer->BeginFrame(eye, view.Transpose(), projection.Transpose(),
                        _mainLight);

#ifdef RenderTest

  std::ranges::for_each(AccessModelData(modelHandle).meshes,
                        [&](MeshHandle meshHandle) {
                          _renderer->BeginDraw(meshHandle, Matrix::Identity);
                          _renderer->DrawMesh(meshHandle);
                        });

#endif // RenderTest
  _renderer->EndFrame();
}
