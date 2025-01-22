#include "GameApp.h"
#include "../../Engine/Source/Renderer/DX11/DX11Renderer.h"
#include "Core/Input/InputSystem.h"
#include "ResourceManager/ResourceManager.h"
#include "WindowManager/WindowManager.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#define RenderTest

//static ModelHandle modelHandle;
//static ModelHandle modelHandle2;
//static ModelHandle modelHandle3;
//static ModelHandle modelHandle4;
//
//
//static ModelHandle skinningTest;
//
//static GameObject* root;
//
//static Animation* anim1;
//static AnimationState* animState1;
//static AnimatorComponent* animComponent1;
//
//static SkeletalMeshComponent* animSkeletal1;
//static SkeletalMeshComponent* animSkeletal2;

//  skinningTest = LoadModel("Models\\SkinningTest\\SkinningTest.gltf");
//
//root = myLevel->CreateGameObjectFromModel<GameObject>(
//    "Models\\SkinningTest\\SkinningTest.gltf");
//
//animSkeletal1 =
//    (*root->childrens.begin())->GetComponent<SkeletalMeshComponent>();
//animSkeletal2 = (*std::next(root->childrens.begin()))
//                    ->GetComponent<SkeletalMeshComponent>();
//
//const ModelData& skinningData = AccessModelData(skinningTest);
//
//for (auto animHandle : skinningData.animations)
//{
//  anim1 = new Animation(animHandle, true);
//}
//
//animState1 = new AnimationState(anim1);
//
//animComponent1 = root->CreateComponent<AnimatorComponent>();
//animComponent1->BineSkeleton(skinningData.skeleton);
//
//animComponent1->SetState(animState1);

//void GameApp::FixedUpdate(float deltaTime)
//{
//  animComponent1->UpdateAnimation(deltaTime);
//
//  animSkeletal1->UpdateBoneTransforms();
//  animSkeletal2->UpdateBoneTransforms();
//}

void GameApp::Initialize(const std::wstring& title)
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
    _renderer->AddShadow(meshHandle);
  });
  std::ranges::for_each(modelData2.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
    _renderer->AddShadow(meshHandle);
    });
  std::ranges::for_each(modelData3.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
  });
  std::ranges::for_each(modelData4.meshes, [&](MeshHandle meshHandle) {
    _renderer->CreateMesh(meshHandle);
    _renderer->AddShadow(meshHandle);
  });
  _camera = new Camera(1920, 1080);
  _mainLight.direction = Vector4(0.f, -1.f, 1.f, 0.f);
  _mainLight.color = Vector4(1.f, 1.f, 1.f, 1.f);
  _mainLight.intensity = Vector4(1.f, 1.f, 1.f, 1.f);


  _camera->SetPosition({0, 0, -50, 1});
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
  if (INPUT->IsKeyPress(Key::Q))
  {
    _camera->MoveDownUp(-deltaTime * 6);
  }
  // 'E' 누르면 Up
  if (INPUT->IsKeyPress(Key::E))
  {
    _camera->MoveDownUp(deltaTime * 6);
  }
  // 'A' 누르면 Left
  if (INPUT->IsKeyPress(Key::A))
  {
    _camera->MoveLeftRight(-deltaTime * 6);
  }
  // 'D' 누르면 Right
  if (INPUT->IsKeyPress(Key::D))
  {
    _camera->MoveLeftRight(deltaTime * 6);
  }
  // 'W' 누르면 Forward
  if (INPUT->IsKeyPress(Key::W))
  {
    _camera->MoveBackForward(deltaTime * 6);
  }
  // 'S' 누르면 Backward
  if (INPUT->IsKeyPress(Key::S))
  {
    _camera->MoveBackForward(-deltaTime * 6);
  }

  if (INPUT->IsKeyDown(Input::MouseState::RB))
  {
    _bCameraMove = !_bCameraMove; // camera bool값이 반대로 됨.
    ShowCursor(!_bCameraMove);    // 커서가 안 보임
  }
  if (INPUT->IsKeyUp(Input::MouseState::RB))
  {
    _bCameraMove = !_bCameraMove; // camera bool값이 반대로 됨.
    ShowCursor(!_bCameraMove);    // 커서가 보임
  }

  // 마우스 회전
  if (_bCameraMove)
  {
    // 마우스 이동량 가져오기
    Vector2 mouseDelta = INPUT->GetMouseDelta();
    float x = -mouseDelta.x * _camera->GetRotationSpeed();
    float y = -mouseDelta.y * _camera->GetRotationSpeed();

   
    if ((INPUT->GetCurrMouseState().x != INPUT->GetPrevMouseState().x) ||
        (INPUT->GetCurrMouseState().y != INPUT->GetPrevMouseState().y))
    {
      // 마우스가 Y 축으로 움직이면 X 축 회전
      _camera->RotateAroundXAxis(y);
      // 마우스가 X 축으로 움직이면 Y 축 회전
      _camera->RotateAroundYAxis(x);
    }
  }
}

void GameApp::Render()
{
  Matrix view = _camera->GetViewTransform();
  Matrix projection = _camera->GetProjectionMatrix();
  _renderer->BeginFrame(_camera->GetPosition(), view, projection,
                        _mainLight);
  if (ImGui::Begin("main Light"))
  {
    float _mainLightDir[3] = {_mainLight.direction.x, _mainLight.direction.y,
                              _mainLight.direction.z};
    ImGui::SliderFloat3("direction", _mainLightDir, -1.f,1.f);
    _mainLight.direction.x = _mainLightDir[0];
    _mainLight.direction.y = _mainLightDir[1];
    _mainLight.direction.z = _mainLightDir[2];
  }
  ImGui::End();
#ifdef RenderTest
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
  Matrix translate3 = Matrix::CreateTranslation(Vector3(100.f, 0.f,.0f));
  world3 *= scale3;
  world3 *= translate3;

  Matrix world4 = Matrix::Identity;
  //Matrix scale4 = Matrix::CreateScale(0.5f);
  Matrix translate4 = Matrix::CreateTranslation(Vector3(0.f, -100.f, .0f));
  //world4 *= scale4;
  world4 *= translate4;
  std::ranges::for_each(AccessModelData(modelHandle).meshes,
                        [&](MeshHandle meshHandle) {
                          _renderer->BeginDraw(meshHandle, world);
                          _renderer->DrawMesh(meshHandle);
                        });
  _renderer->DrawDebugCylinder(
      Matrix::CreateScale(35.f) *
          Matrix::CreateFromQuaternion(
              Quaternion::CreateFromAxisAngle(Vector3(1.f, 0.f, 0.f),
                                              XMConvertToRadians(90.f)))*
          Matrix::CreateTranslation(Vector3(0.f, -10.f, 0.0f)),
      Color(0.f, 1.f, 0.f));
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
  _renderer->DrawDebugSphere(
      Matrix::CreateScale(30.f) *
          Matrix::CreateTranslation(Vector3(100.f, 0.f, .0f)),
      Color(1.f, 0.f, 0.f));
  _renderer->DrawDebugBox(Matrix::CreateScale({230.f,50.f,50.f})
                          *Matrix::CreateTranslation(-180.f, 0.f, 0.0f)
                          , Color(0.f, 0.f, 1.f));
  std::ranges::for_each(AccessModelData(modelHandle4).meshes,
                        [&](MeshHandle meshHandle) {
                          _renderer->BeginDraw(meshHandle, world4);
                          _renderer->DrawMesh(meshHandle);
                        });



#endif // RenderTest
  _renderer->EndFrame();
}
