#include "GameApp.h"

#include "../../../PhyjixEngine/PhyjixWorld.h"
#include "../../Engine/Source/Renderer/DX11/DX11Renderer.h"
#include "Core/Input/InputSystem.h"
#include "GameFramework/Components/RigidbodyComponent.h"
#include "GameFramework/Level/Level.h"
#include "GameFramework/World/World.h"
#include "ResourceManager/ResourceManager.h"
#include "WindowManager/WindowManager.h"

#include "GameFramework/Components/Animation/Animation.h"
#include "GameFramework/Components/Animation/AnimationState.h"
#include "GameFramework/Components/Animation/AnimatorComponent.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "../../PhyjixEngine/IPhyjixEngine.h"
#include "../../PhyjixEngine/PhyjixEngine.h"
#include "../../PhyjixEngine/IPhyjixWorld.h"

#define RenderTest

static ModelHandle modelHandle;
static ModelHandle modelHandle2;
static ModelHandle modelHandle3;
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

void GameApp::FixedUpdate(float deltaTime)
{
  animComponent1->UpdateAnimation(deltaTime);

  animSkeletal1->UpdateBoneTransforms();
  animSkeletal2->UpdateBoneTransforms();
}

void GameApp::Initialize(UINT screenWidth, UINT screenHeight,
                         const std::wstring& title)
{
  _phyjixEngine = new PhyjixEngine();
  _phyjixEngine->Initialize();
  _phyjixWorld = _phyjixEngine->CreateWorld();
  _phyjixWorld->CreateDefaultGround();

  _renderer = new DX11Renderer;
  Super::Initialize(screenWidth, screenHeight, title);
  _renderer->Init_Win32(screenWidth, screenHeight, nullptr, &_hwnd);
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
  _mainLight.radiance = Vector4(1.f, 1.f, 1.f, 1.f);

  _camera->SetPosition({0, 0, -50, 1});
  at = Vector4::Zero;

  myWorld = new World();
  myLevel = myWorld->CreateLevel<Level>("Test Level");

  skinningTest = LoadModel("Models\\SkinningTest\\SkinningTest.gltf");

  root = myLevel->CreateGameObjectFromModel<GameObject>(
      "Models\\SkinningTest\\SkinningTest.gltf");

  animSkeletal1 =
      (*root->childrens.begin())->GetComponent<SkeletalMeshComponent>();
  animSkeletal2 = (*std::next(root->childrens.begin()))
                      ->GetComponent<SkeletalMeshComponent>();

  const ModelData& skinningData = AccessModelData(skinningTest);

  for (auto animHandle : skinningData.animations)
  {
    anim1 = new Animation(animHandle, true);
  }

  animState1 = new AnimationState(anim1);

  animComponent1 = root->CreateComponent<AnimatorComponent>();
  animComponent1->BineSkeleton(skinningData.skeleton);

  animComponent1->SetState(animState1);
  testobject2 = new GameObject(myWorld);
  testobject2->CreateComponent<RigidbodyComponent>();
  testrigidbody2 = testobject2->GetComponent<RigidbodyComponent>();
  testrigidbody2->Initialize({0,10,0}, {10,10,10},
                            ColliderShape::eCubeCollider, true, false,
                            static_cast<PhyjixWorld*>(_phyjixWorld));
  testrigidbody2->EnableDebugDraw();
 

  testobject = new GameObject(myWorld);
  testobject->CreateComponent<RigidbodyComponent>();
  testrigidbody = testobject->GetComponent<RigidbodyComponent>();
  testrigidbody->Initialize({0, 100, 0}, {10, 10, 10},
                            ColliderShape::eCubeCollider, false, false,
                            static_cast<PhyjixWorld*>(_phyjixWorld));
  testrigidbody->EnableDebugDraw();
  testrigidbody->SetCollisionEvent(testrigidbody2->GetRigidBody(),
                                   eCollisionEventType::eCollisionEnter, [&]() 
                                   {
                                     testrigidbody->DisableDebugDraw();
                                     testrigidbody2->DisableDebugDraw();
                                   });





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

void GameApp::Update(float deltaTime)
{
  _phyjixEngine->Update(deltaTime);
  // _camera->LookAt({10, 0, 0, 1});
  // 'Q' 누르면 Down
  if (Input.IsKeyPress(Key::Q))
  {
    _camera->MoveDownUp(-deltaTime * 6);
  }
  // 'E' 누르면 Up
  if (Input.IsKeyPress(Key::E))
  {
    _camera->MoveDownUp(deltaTime * 6);
  }
  // 'A' 누르면 Left
  if (Input.IsKeyPress(Key::A))
  {
    _camera->MoveLeftRight(-deltaTime * 6);
  }
  // 'D' 누르면 Right
  if (Input.IsKeyPress(Key::D))
  {
    _camera->MoveLeftRight(deltaTime * 6);
  }
  // 'W' 누르면 Forward
  if (Input.IsKeyPress(Key::W))
  {
    _camera->MoveBackForward(deltaTime * 6);
  }
  // 'S' 누르면 Backward
  if (Input.IsKeyPress(Key::S))
  {
    _camera->MoveBackForward(-deltaTime * 6);
  }
  if (Input.IsKeyPress(Key::T))
  {
    /*if (testrigidbody->GetDebugDrawFlag())
      testrigidbody->DisableDebugDraw();
    else
      testrigidbody->EnableDebugDraw();*/
    testrigidbody->DisableGravity();
  } 

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
  
  Matrix view = _camera->GetViewTransform();
  Matrix projection = _camera->GetProjectionMatrix();
  _renderer->BeginFrame(_camera->GetPosition(), view, projection, _mainLight);
  if (ImGui::Begin("main Light"))
  {
    float _mainLightDir[3] = {_mainLight.direction.x, _mainLight.direction.y,
                              _mainLight.direction.z};
    ImGui::SliderFloat3("direction", _mainLightDir, -1.f, 1.f);
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
  if (testrigidbody->GetDebugDrawFlag())
  {
    _renderer->DrawDebugBox(Matrix::CreateScale({20, 20, 20}) * 
        Matrix::CreateFromQuaternion(
                testrigidbody->GetRigidBody()->GetWorldRotation()) *
                                Matrix::CreateTranslation(testrigidbody->GetRigidBody()->GetWorldPosition()),
                            Color(1, 1, 0));
  }
  if (testrigidbody2->GetDebugDrawFlag())
  {
    _renderer->DrawDebugBox(
        Matrix::CreateScale({20, 20, 20}) *
            Matrix::CreateFromQuaternion(
                testrigidbody2->GetRigidBody()->GetWorldRotation()) *
            Matrix::CreateTranslation(
                testrigidbody2->GetRigidBody()->GetWorldPosition()),
        Color(1, 0, 1));
  }

  /* animSkeletal1->boneTransforms;
   _renderer
       ->BeginDraw(animSkeletal1->GetHandle(), )
   animSkeletal1*/

#endif // RenderTest
  _renderer->EndFrame();
}
