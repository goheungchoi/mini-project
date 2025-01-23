#include "GameApp.h"

#include "Core/Time/TimeSystem.h"
#include "Core/Input/InputSystem.h"
#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"
#include "ResourceManager/ResourceManager.h"
#include "WindowManager/WindowManager.h"

#include "GameFramework/World/World.h"
#include "GameFramework/Level/Level.h"
#include "Contents/Levels/TestLevel.h"

static TestLevel* testLevel;

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#define RenderTest
//#define USED2D

static ModelHandle modelHandle;
static ModelHandle modelHandle2;
static ModelHandle modelHandle4;

static ModelHandle skinningTest;

static GameObject* root;

static Animation* anim1;
static AnimationState* animState1;
static AnimatorComponent* animComponent1;

static SkeletalMeshComponent* animSkeletal1;
static SkeletalMeshComponent* animSkeletal2;

static World* myWorld;
static Level* myLevel;

void GameApp::Initialize(UINT screenWidth, UINT screenHeight,
                         const std::wstring& title)
{
  _renderer = new DX11Renderer;
  Super::Initialize(screenWidth, screenHeight, title);
  _renderer->Init_Win32(screenWidth, screenHeight, nullptr, &_hwnd);
#ifdef RenderTest
#endif // RenderTest
  modelHandle = LoadModel("Models\\FlightHelmet\\FlightHelmet.gltf");
  modelHandle2 = LoadModel("Models\\Ceberus\\Ceberus.glb");
  modelHandle4 = LoadModel("Models\\Sponza\\Sponza.gltf");
  ModelData modelData = AccessModelData(modelHandle);
  ModelData modelData2 = AccessModelData(modelHandle2);
  ModelData modelData4 = AccessModelData(modelHandle4);
  // skybox init
  _renderer->CreateSkyBox(
      "Textures/BakerEnv.dds", "Textures/BakerSpecularBRDF_LUT.dds",
      "Textures/BakerDiffuseIrradiance.dds", "Textures/BakerSpecularIBL.dds");
  std::ranges::for_each(modelData.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
    _renderer->AddShadow(meshHandle);
  });
  std::ranges::for_each(modelData2.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
    _renderer->AddShadow(meshHandle);
  });
  std::ranges::for_each(modelData4.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
    _renderer->AddShadow(meshHandle);
  });
  _camera = new Camera(1920, 1080);
  _mainLight.direction = Vector4(0.f, -1.f, 1.f, 0.f);
  _mainLight.radiance = Vector4(1.f, 1.f, 1.f, 1.f);

  _camera->SetPosition({0, 0, -50, 1});
  at = Vector4::Zero;

  myWorld = new World();
  myLevel = myWorld->CreateLevel<Level>("Test Level");

  skinningTest = LoadModel("Models\\SkinningTest\\SkinningTest.fbx");

  root = myLevel->CreateGameObjectFromModel<GameObject>(
      "Models\\SkinningTest\\SkinningTest.fbx");

  animSkeletal1 = (*std::next(root->childrens.begin()))
                      ->GetComponent<SkeletalMeshComponent>();
  animSkeletal2 = (*std::next(std::next(root->childrens.begin())))
                      ->GetComponent<SkeletalMeshComponent>();

  const ModelData& skinningData = AccessModelData(skinningTest);

  for (auto animHandle : skinningData.animations)
  {
    anim1 = new Animation(animHandle, true);
  }

  _world = World::CreateWorld(_hwnd, title);

  testLevel = new TestLevel();

  _world->AddLevel(testLevel);
  _world->PrepareChangeLevel("Test Level");
  _world->CommitLevelChange();
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

void GameApp::ProcessInput(float dt) {
  _world->ProcessInput(dt);
}

void GameApp::FixedUpdate(float fixedRate) {
  _world->FixedUpdate(fixedRate);
  _world->PhysicsUpdate(fixedRate);
}

void GameApp::Update(float dt)
{
  _world->ProcessInput(dt);
  _world->PreUpdate(dt);
  _world->Update(dt);
  _world->AnimationUpdate(dt);
  _world->PostUpdate(dt);
  if (Input.IsKeyDown(MouseState::RB))
  {
    bCameraMove = !bCameraMove; // camera bool값이 반대로 됨.
    ShowCursor(!bCameraMove);   // 커서가 안 보임
  }
  if (Input.IsKeyUp(MouseState::RB))
  {
    bCameraMove = !bCameraMove; // camera bool값이 반대로 됨.
    ShowCursor(!bCameraMove);   // 커서가 보임
  }

  // 마우스 회전
  if (bCameraMove)
  {
    // 마우스 이동량 가져오기
    Vector2 mouseDelta = Input.GetMouseDelta();
    float x = -mouseDelta.x * _camera->GetRotationSpeed();
    float y = -mouseDelta.y * _camera->GetRotationSpeed();

    if ((Input.GetCurrMouseState().x != Input.GetPrevMouseState().x) ||
        (Input.GetCurrMouseState().y != Input.GetPrevMouseState().y))
    {
      // 마우스가 Y 축으로 움직이면 X 축 회전
      _camera->RotateAroundXAxis(y);
      // 마우스가 X 축으로 움직이면 Y 축 회전
      _camera->RotateAroundYAxis(x);
    }
  }

  animComponent1->UpdateAnimation(deltaTime);

  animSkeletal1->UpdateBoneTransforms();
  animSkeletal2->UpdateBoneTransforms();
}

void GameApp::Render()
{
  _world->RenderGameObjects();
  _world->RenderUI();
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
  _renderer->DrawDebugCylinder(
      Matrix::CreateScale(35.f) *
          Matrix::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(
              Vector3(1.f, 0.f, 0.f), XMConvertToRadians(90.f))) *
          Matrix::CreateTranslation(Vector3(0.f, -10.f, 0.0f)),
      Color(0.f, 1.f, 0.f));
  std::ranges::for_each(AccessModelData(modelHandle2).meshes,
                        [&](MeshHandle meshHandle) {
                          _renderer->BeginDraw(meshHandle, world2);
                          _renderer->DrawMesh(meshHandle);
                        });

  std::ranges::for_each(AccessModelData(modelHandle4).meshes,
                        [&](MeshHandle meshHandle) {
                          _renderer->BeginDraw(meshHandle, world4);
                          _renderer->DrawMesh(meshHandle);
                        });
  Matrix skelworld1 = Matrix::Identity * Matrix::CreateScale(0.6f) *
                      Matrix::CreateTranslation(100.f, -30.f, 0.f);
  Matrix skelworld2 = Matrix::Identity * Matrix::CreateScale(0.6f) *
                      Matrix::CreateTranslation(100.f, -30.f, 0.f);
  _renderer->BeginDraw(animSkeletal1->GetHandle(), skelworld1);
  _renderer->DrawMesh(animSkeletal1->GetHandle(),
                      animSkeletal1->boneTransforms);
  _renderer->BeginDraw(animSkeletal2->GetHandle(), skelworld2);
  _renderer->DrawMesh(animSkeletal2->GetHandle(),
                      animSkeletal2->boneTransforms);

#endif // RenderTest

#ifdef USED2D
  _renderer->TextDraw(L"한글Test1234", {0, 0, 1920, 150}, L"궁서");
#endif // USED2D


  _renderer->EndFrame();
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

void GameApp::Run()
{
  TimeSystem::Reset();

  double dt = 0.0;           // 델타 타임
  double dtThreshole = 0.25; // 최대 델타 타임
  double fixedRate = kFixedRate;    // 픽스 타임
  double accumulator = 0.0;         // 픽스 타임 트레킹

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
      
      dt = TimeSystem::GetDeltaTime();
      
      InputSystem::GetInstance()->Update(dt);

      if (_world->changingLevel)
        continue;

      _world->InitialStage();

      // 게임 루프 소용돌이를 방어하기 위해 델타타임이 일정시간 이상 넘어가면
      // 픽스 합니다.
      dtThreshole = 0.25 * dt;
      if (dt > dtThreshole)
        dt = dtThreshole; // Clamp the frame time


      // 프레임 타임을 누적해서 픽스 타임을 추적합니다.
      accumulator += dt;
      // 일정 시간마다 픽스 업데이트를 실행합니다.
      fixedRate = kFixedRate * dt; // 스케일 된 픽스 타임
      while (accumulator >= fixedRate)
      {
        FixedUpdate(fixedRate); // Update physics at a fixed rate
        accumulator -= fixedRate;
      }
      
      Update(dt);
      Render();

      _world->CleanupStage();
    }
  }
}
