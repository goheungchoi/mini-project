#include "GameApp.h"
#include "../../Engine/Source/Renderer/DX11/DX11Renderer.h"
#include "Core/Input/InputSystem.h"
#include "ResourceManager/ResourceManager.h"
#include "WindowManager/WindowManager.h"
#define ClientTest

static ModelHandle modelHandle;
static ModelHandle modelHandle2;
static ModelHandle modelHandle3;
static ModelHandle modelHandle4;
void GameApp::Initialize()
{
#ifdef ClientTest
  // ����׿� �ܼ�â ���� *Debug*
  bool bUseConsole = true; // �̰� true�� �ٲٸ� �ܼ�â ��.
  if (bUseConsole)
  {
    AllocConsole();
    FILE* _tempFile;
    freopen_s(&_tempFile, "CONOUT$", "w", stdout);
  }
#endif // DEBUG

  // ������ ����
  _hwnd =
      WindowManager::GetInstance()->CreateWinApp(1280, 720, L"Client Test");
  _renderer = new DX11Renderer;
  
  Super::Initialize();


  _renderer->Init_Win32(1280, 720, nullptr, &_hwnd);
#ifdef ClientTest
#endif // RenderTest
  modelHandle = LoadModel("Models\\FlightHelmet\\FlightHelmet.gltf");
  modelHandle2 = LoadModel("Models\\Ceberus\\Ceberus.glb");
  modelHandle3 = LoadModel("Models\\Sphere\\Sphere.obj");
  modelHandle4 = LoadModel("Models\\Sponza\\Sponza.gltf");
  ModelData modelData = AccessModelData(modelHandle);
  ModelData modelData2 = AccessModelData(modelHandle2);
  ModelData modelData3 = AccessModelData(modelHandle3);
  ModelData modelData4 = AccessModelData(modelHandle4);
  // skybox init
  _renderer->CreateSkyBox(
      "Textures/BakerEnv.dds", "Textures/BakerSpecularBRDF_LUT.dds",
      "Textures/BakerDiffuseIrradiance.dds", "Textures/BakerSpecularIBL.dds");
  std::ranges::for_each(modelData.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
  });
  std::ranges::for_each(modelData2.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
  });
  std::ranges::for_each(modelData3.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
  });
  std::ranges::for_each(modelData4.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
  });
  _camera = new Camera(1920, 1080);
  _mainLight.direction = Vector4(0.f, 0.f, 1.f, 0.f);
  _mainLight.color = Vector4(1.f, 1.f, 1.f, 1.f);
  _mainLight.intensity = Vector4(1.f, 1.f, 1.f, 1.f);

  _camera->SetPosition({4.367269, -0.90219879, -40.523827, +76.853645});
  eye = {4.367269, -0.90219879, -40.523827, +76.853645};
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

  // 'Q' ������ Down
  if (INPUT->IsKeyPress(Key::Q))
  {
    _camera->MoveDownUp(-deltaTime * 6);
  }
  // 'E' ������ Up
  if (INPUT->IsKeyPress(Key::E))
  {
    _camera->MoveDownUp(deltaTime * 6);
  }
  // 'A' ������ Left
  if (INPUT->IsKeyPress(Key::A))
  {
    _camera->MoveLeftRight(-deltaTime * 6);
  }
  // 'D' ������ Right
  if (INPUT->IsKeyPress(Key::D))
  {
    _camera->MoveLeftRight(deltaTime * 6);
  }
  // 'W' ������ Forward
  if (INPUT->IsKeyPress(Key::W))
  {
    _camera->MoveBackForward(deltaTime * 6);
  }
  // 'S' ������ Backward
  if (INPUT->IsKeyPress(Key::S))
  {
    _camera->MoveBackForward(-deltaTime * 6);
  }

  if (INPUT->IsKeyDown(Input::MouseState::RB))
  {
    _bCameraMove = !_bCameraMove; // camera bool���� �ݴ�� ��.
    ShowCursor(!_bCameraMove);    // Ŀ���� �� ����
  }
  if (INPUT->IsKeyUp(Input::MouseState::RB))
  {
    _bCameraMove = !_bCameraMove; // camera bool���� �ݴ�� ��.
    ShowCursor(!_bCameraMove);    // Ŀ���� ����
  }

  // ���콺 ȸ��
  if (_bCameraMove)
  {
    // ���콺 �̵��� ��������
    Vector2 mouseDelta = INPUT->GetMouseDelta();
    float x = -mouseDelta.x * _camera->GetRotationSpeed();
    float y = -mouseDelta.y * _camera->GetRotationSpeed();

    if ((INPUT->GetCurrMouseState().x != INPUT->GetPrevMouseState().x) ||
        (INPUT->GetCurrMouseState().y != INPUT->GetPrevMouseState().y))
    {
      // ���콺�� Y ������ �����̸� X �� ȸ��
      _camera->RotateAroundXAxis(y);
      // ���콺�� X ������ �����̸� Y �� ȸ��
      _camera->RotateAroundYAxis(x);
    }
  }
}

void GameApp::Render()
{
  Matrix view = _camera->GetViewTransform();
  Matrix projection = _camera->GetProjectionMatrix();
  _renderer->BeginFrame(_camera->GetPosition(), view, projection, _mainLight);
#ifdef ClientTest
  Matrix world = Matrix::Identity;
  Matrix scale = Matrix::CreateScale(100.f);
  Matrix translate = Matrix::CreateTranslation(Vector3(0.f, -50.f, 0.0f));

  world *= scale;
  world *= translate;
  Matrix world2 = Matrix::Identity;
  Matrix scale2 = Matrix::CreateScale(1.5f);
  Matrix translate2 = Matrix::CreateTranslation(Vector3(-100.f, 0.f, 0.0f));
  Quaternion rotation = Quaternion::CreateFromAxisAngle(
      Vector3(0.f, 1.f, 0.f), XMConvertToRadians(90.f));
  Matrix rotationMatrix = Matrix::CreateFromQuaternion(rotation);
  world2 *= scale2;
  world2 *= rotationMatrix;
  world2 *= translate2;

  Matrix world3 = Matrix::Identity;
  Matrix scale3 = Matrix::CreateScale(20.f);
  Matrix translate3 = Matrix::CreateTranslation(Vector3(100.f, 0.f, .0f));
  world3 *= scale3;
  world3 *= translate3;

  Matrix world4 = Matrix::Identity;
  // Matrix scale4 = Matrix::CreateScale(0.5f);
  Matrix translate4 = Matrix::CreateTranslation(Vector3(0.f, -100.f, .0f));
  // world4 *= scale4;
  world4 *= translate4;
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
  std::ranges::for_each(AccessModelData(modelHandle3).meshes,
                        [&](MeshHandle meshHandle) {
                          _renderer->BeginDraw(meshHandle, world3);
                          _renderer->DrawMesh(meshHandle);
                        });
  std::ranges::for_each(AccessModelData(modelHandle4).meshes,
                        [&](MeshHandle meshHandle) {
                          _renderer->BeginDraw(meshHandle, world4);
                          _renderer->DrawMesh(meshHandle);
                        });

#endif // ClientTest
  _renderer->EndFrame();
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
