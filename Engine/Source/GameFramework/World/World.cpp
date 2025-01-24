#include "GameFramework/World/World.h"

#include "Renderer/DX11/DX11Renderer.h"

#include "Core/Input/InputSystem.h"
#include "Core/Camera/Camera.h"

#include "GameFramework/Components/Animation/AnimatorComponent.h"
#include "GameFramework/Components/LightComponent.h"
#include "GameFramework/Components/MeshComponent.h"
#include "GameFramework/Components/SkeletalMeshComponent.h"
#include "GameFramework/Components/TransformComponent.h"

#ifdef _DEBUG
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#endif
void World::Initialize(HWND hwnd, const std::wstring& title) {
  _renderer = new DX11Renderer();
  _renderer->Init_Win32(kScreenWidth, kScreenHeight, nullptr, &hwnd);
  _renderer->CreateSkyBox(
      "Textures/BakerEnv.dds", "Textures/BakerSpecularBRDF_LUT.dds",
      "Textures/BakerDiffuseIrradiance.dds", "Textures/BakerSpecularIBL.dds");

  _defaultCamera = new Camera(kScreenWidth, kScreenHeight,XM_PIDIV4/4*6);
  SetMainCamera(_defaultCamera);
}

World* World::CreateWorld(HWND hwnd, const std::wstring& title)
{
  World* newWorld = new World();
  newWorld->Initialize(hwnd, title);

  return newWorld;
}

void World::PrepareChangeLevel(const std::string& levelName) {
  /* TODO: */
  if (auto it = _levelMap.find(levelName); it != _levelMap.end())
  {
    _preparingLevel = it->second;
    
    _preparingLevel->PrepareLevel();
  }
  else
  {
    throw std::runtime_error("Level is not found: " + levelName);
  }
}

bool World::IsLevelChangeReady() {
  if (!_preparingLevel)
    return false;

  /* TODO: */
  auto& f = _levelPreparedMap[_preparingLevel->name];

  return f.valid() && f.wait_for(0s) == std::future_status::ready && f.get();
}

void World::CommitLevelChange() {
  //async::executor.async([&]() {
  //  auto& f = _levelPreparedMap[_preparingLevel->name];
  //  f.get();

  //  while (!readyToChangeLevel) {}  // Wait until
    changingLevel = true;
    
    if (_currentLevel)
    {
      _currentLevel->DestroyLevel();
      _currentLevel->CleanupLevel();
    }
    
    _currentLevel = _preparingLevel;
    _currentLevel->BeginLevel();

    changingLevel = false;
  // });
}

void World::AddLevel(Level* level) {
  level->world = this;
  _levelMap[level->name] = level;
}

void World::SetMainCamera(Camera* camera) {
  if (camera)
    mainCamera = camera;
  else
    mainCamera = _defaultCamera;
}

void World::StopCameraMovement(bool fixed)
{
  bCameraMove = !fixed;
}

XMVECTOR World::ScreenToWorldPosition(XMVECTOR screenPos)
{


  return XMVECTOR();
}

void World::RegisterGameObjectName(GameObject* gameObject) {
  if (!_currentLevel)
    return;
}

void World::UnregisterGameObjectName(GameObject* gameObject)
{
  if (!_currentLevel)
    return;
}

void World::RegisterGameObjectTag(GameObject* gameObject)
{
  if (!_currentLevel)
    return;
}

void World::UnregisterGameObjectTag(GameObject* gameObject)
{
  if (!_currentLevel)
    return;
}

void World::RegisterMeshComponent(MeshComponent* meshComp)
{
  if (!meshComp)
    return;

  _renderer->CreateMesh(meshComp->GetHandle());
  _renderer->AddShadow(meshComp->GetHandle());
}

void World::RegisterMeshComponent(SkeletalMeshComponent* skeletalMeshComp) {
  if (!skeletalMeshComp)
    return;

  _renderer->CreateMesh(skeletalMeshComp->GetHandle());
}

void World::InitialStage() {
  if (!_currentLevel || changingLevel)
    return;

  // Mark that it should not change the level while running the game loop.
  readyToChangeLevel = false;

  // Awake and activate game objects.
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (gameObject->status == EStatus_Awake && gameObject->bShouldActivate)
    {
      // Awake the game object and activate it.
      gameObject->OnAwake();
      gameObject->OnActivated();
      gameObject->isActive = true;
      gameObject->status = EStatus_Active;
      gameObject->bShouldActivate = false;
    } 

    if (gameObject->status == EStatus_Inactive && gameObject->bShouldActivate)
    {
      // Activate the game object.
      gameObject->OnActivated();
      gameObject->isActive = true;
      gameObject->status = EStatus_Active;
      gameObject->bShouldActivate = false;
    }
  }
}

void World::FixedUpdate(float fixedRate)
{
  if (!_currentLevel || changingLevel)
    return;

  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!gameObject->status == EStatus_Active)
      continue;

    gameObject->FixedUpdate(fixedRate);
  }
}

void World::PhysicsUpdate(float fixedRate) {
  if (!_currentLevel || changingLevel)
    return;

  // TODO:
}

void World::ProcessInput(float dt)
{
  // _camera->LookAt({10, 0, 0, 1});
  // 'Q' 누르면 Down
  if (Input.IsKeyPress(Key::Q))
  {
    mainCamera->MoveDownUp(-dt);
  }
  // 'E' 누르면 Up
  if (Input.IsKeyPress(Key::E))
  {
    mainCamera->MoveDownUp(dt);
  }
  // 'A' 누르면 Left
  if (Input.IsKeyPress(Key::A))
  {
    mainCamera->MoveLeftRight(-dt);
  }
  // 'D' 누르면 Right
  if (Input.IsKeyPress(Key::D))
  {
    mainCamera->MoveLeftRight(dt);
  }
  // 'W' 누르면 Forward
  if (Input.IsKeyPress(Key::W))
  {
    mainCamera->MoveBackForward(dt);
  }
  // 'S' 누르면 Backward
  if (Input.IsKeyPress(Key::S))
  {
    mainCamera->MoveBackForward(-dt);
  }
  if (Input.IsKeyPress(Key::OemMinus))
  {
    mainCamera->AddMoveSpeed(-10);
  }
  if (Input.IsKeyPress(Key::OemPlus))
  {
    mainCamera->AddMoveSpeed(+10);
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
    float x = -mouseDelta.x * mainCamera->GetRotationSpeed();
    float y = -mouseDelta.y * mainCamera->GetRotationSpeed();

    if ((Input.GetCurrMouseState().x != Input.GetPrevMouseState().x) ||
        (Input.GetCurrMouseState().y != Input.GetPrevMouseState().y))
    {
      // 마우스가 Y 축으로 움직이면 X 축 회전
      mainCamera->RotateAroundXAxis(y);
      // 마우스가 X 축으로 움직이면 Y 축 회전
      mainCamera->RotateAroundYAxis(x);
    }
  }
}

void World::PreUpdate(float dt) {
  if (!_currentLevel || changingLevel)
    return;

  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!gameObject->status == EStatus_Active)
      continue;

    gameObject->PreUpdate(dt);
  }
}

void World::Update(float dt)
{
  if (!_currentLevel || changingLevel)
    return;

  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!gameObject->status == EStatus_Active)
      continue;

    gameObject->Update(dt);
  }
}

void World::AnimationUpdate(float dt)
{
  if (!_currentLevel || changingLevel)
    return;

  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!gameObject->status == EStatus_Active)
      continue;

    if (auto* animComp = gameObject->GetComponent<AnimatorComponent>();
        animComp)
    {
      animComp->UpdateAnimation(dt);
    }
  }
}

void World::PostUpdate(float dt)
{
  if (!_currentLevel || changingLevel)
    return;

  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!gameObject->status == EStatus_Active)
      continue;

    gameObject->PostUpdate(dt);
  }
}
static DirectionalLight _mainLight;
void World::RenderGameObjects() {
  if (!_currentLevel || changingLevel)
    return;

  Matrix view = mainCamera->GetViewTransform();
  Matrix projection = mainCamera->GetProjectionMatrix();
  _renderer->BeginFrame(mainCamera->GetPosition(), view, projection,
                        _mainLight);
  #ifdef _DEBUG
  if (ImGui::Begin("main Light"))
  {
    float _mainLightDir[3] = {_mainLight.direction.x, _mainLight.direction.y,
                              _mainLight.direction.z};
    ImGui::SliderFloat3("direction", _mainLightDir, -1.f, 1.f);
    _mainLight.direction.x = _mainLightDir[0];
    _mainLight.direction.y = _mainLightDir[1];
    _mainLight.direction.z = _mainLightDir[2];
    _mainLight.radiance = {1.f, 1.f, 1.f, 1.f};
  }
  ImGui::End();
  #endif
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (!gameObject->status == EStatus_Active)
      continue;

    if (auto* meshComp = gameObject->GetComponent<MeshComponent>();
        meshComp)
    {
      auto handle = meshComp->GetHandle();
      const auto& transform = gameObject->GetWorldTransform();
      _renderer->BeginDraw(handle, transform*XMMatrixScaling(100.f,100.f,100.f));
      _renderer->DrawMesh(handle);
    }
    else if (auto* meshComp = gameObject->GetComponent<SkeletalMeshComponent>();
             meshComp)
    {
      // TODO:
      /*auto handle = meshComp->GetHandle();
      const auto& transform = gameObject->GetWorldTransform();
      _renderer->BeginDraw(handle, transform);
      _renderer->DrawMesh(handle);*/
    }
  }

  _renderer->EndFrame();
}

void World::RenderUI() {
  // TODO:
}

void World::CleanupStage() {
  if (!_currentLevel || changingLevel)
    return;

  // Find cleaned up game objects.
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (gameObject->status == EStatus_Cleanup)
    {
      // Mark as destroyed.
      gameObject->FinishDestroy();
    }
  }

  // Deactivation & cleanup components
  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    if (gameObject->status == EStatus_Active && gameObject->bShouldDeactivate)
    {
      // Awake the game object and activate it.
      gameObject->status = EStatus_Inactive;
      gameObject->isActive = false;
      gameObject->bShouldDeactivate = false;
    }

    if (gameObject->bShouldDestroy)
    {
      // Activate the game object.
      gameObject->BeginDestroy();
      gameObject->isActive = false;
      gameObject->bShouldDestroy = false;
    }
  }

  // Destroy game objects
  for (auto it = _currentLevel->GetGameObjectList().begin();
       it != _currentLevel->GetGameObjectList().end();)
  {
    GameObject* gameObject = *it;
    if (gameObject->status == EStatus_Destroyed)
    {
       it = _currentLevel->gameObjects.erase(it);
    }
    else
    {
      ++it;
    }
  }

  // Mark that it's ready to change level.
  readyToChangeLevel = true;
}


