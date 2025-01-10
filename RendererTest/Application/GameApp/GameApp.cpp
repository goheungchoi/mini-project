#include "GameApp.h"
#include "../../Engine/Source/Renderer/DX11/DX11Renderer.h"
#include "ResourceManager/ResourceManager.h"
#include "WindowManager/WindowManager.h"
#define RenderTest

static ModelHandle modelHandle;
static ModelHandle modelHandle2;
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
  modelHandle = LoadModel("Models\\FlightHelmet\\FlightHelmet.gltf");
  modelHandle2 = LoadModel("Models\\Ceberus\\Ceberus.glb");
  ModelData modelData = AccessModelData(modelHandle);
  ModelData modelData2 = AccessModelData(modelHandle2);

  std::ranges::for_each(modelData.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
  });
  std::ranges::for_each(modelData.meshes, [&](MeshHandle meshHandle) {
    _renderer->AddRenderPass(meshHandle, RenderPassType::TransparentPass);
  });
  std::ranges::for_each(modelData2.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
  });
  std::ranges::for_each(modelData2.meshes, [&](MeshHandle meshHandle) {
    _renderer->AddRenderPass(meshHandle, RenderPassType::TransparentPass);
  });
  _camera = new Camera(1920,1080);
  _mainLight.direction = Vector4(-1.f, 0.f, 0.f, 0.f);
  _mainLight.color = Vector4(1.f, 1.f, 1.f, 1.f);
  _mainLight.intensity = Vector4(1.f, 1.f, 1.f, 1.f);

  eye = {70.f, 0.0f, 0.f, 1.f};
  at = Vector4::Zero;
}

void GameApp::Execute()
{
  Super::Execute();
}

void GameApp::Shutdown()
{
  _renderer->CreatePipeline();
  _renderer->Cleanup();
  delete _camera;
  delete _renderer;
  Super::Shutdown();
}

void GameApp::FixedUpdate(float deltaTime) {}

void GameApp::Update(float deltaTime)
{

  // 'Q' 누르면 Down
  if (GetAsyncKeyState('Q') & 0x8000)
  {
    _camera->MoveDownUp(-deltaTime);
  }
  // 'E' 누르면 Up
  if (GetAsyncKeyState('E') & 0x8000)
  {
    _camera->MoveDownUp(deltaTime);
  }
  // 'A' 누르면 Left
  if (GetAsyncKeyState('A') & 0x8000)
  {
    _camera->MoveLeftRight(-deltaTime);
  }
  // 'D' 누르면 Right
  if (GetAsyncKeyState('D') & 0x8000)
  {
    _camera->MoveLeftRight(deltaTime);
  }
  // 'W' 누르면 Forward
  if (GetAsyncKeyState('W') & 0x8000)
  {
    _camera->MoveBackForward(deltaTime);
  }
  // 'S' 누르면 Backward
  if (GetAsyncKeyState('S') & 0x8000)
  {
    _camera->MoveBackForward(-deltaTime);
  }
}

void GameApp::Render()
{
  Matrix view = _camera->GetViewTransform();
  Matrix projection = _camera->GetProjectionMatrix();
  _renderer->BeginFrame(eye, view, projection, _mainLight);
#ifdef RenderTest
  Matrix world = Matrix::Identity;
  Matrix scale = Matrix::CreateScale(100.f);
  Matrix translate = Matrix::CreateTranslation(Vector3(0.f, -50.f, 0.0f));
  Quaternion rotation = Quaternion::CreateFromAxisAngle(
      Vector3(0.f, 1.f, 0.f), XMConvertToRadians(-90.f));
  Matrix rotationMatrix = Matrix::CreateFromQuaternion(rotation);
  world *= rotationMatrix;

  world *= scale;
  world *= translate;
  Matrix world2 = Matrix::Identity;
  Matrix scale2 = Matrix::CreateScale(1.5f);
  Matrix translate2 = Matrix::CreateTranslation(Vector3(0.f, 0.f, 230.0f));
  world2 *= scale2;
  world2 *= translate2;
  std::ranges::for_each(AccessModelData(modelHandle).meshes,
                        [&](MeshHandle meshHandle) {
                          _renderer->BeginDraw(meshHandle, world);
                          _renderer->DrawMesh(meshHandle);
                        });
  std::ranges::for_each(AccessModelData(modelHandle2).meshes,
                        [&](MeshHandle meshHandle) {
                          _renderer->BeginDraw(meshHandle, world2);
                          _renderer->DrawMesh(meshHandle);
                        });

#endif // RenderTest
  _renderer->EndFrame();
}
