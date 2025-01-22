#include "GameFramework/World/World.h"

#include "Renderer/DX11/DX11Renderer.h"

#include "Core/Input/InputSystem.h"
#include "Core/Camera/Camera.h"

void World::Initialize(HWND hwnd, const std::wstring& title) {
  _renderer = new DX11Renderer();
  _renderer->Init_Win32(kScreenWidth, kScreenHeight, nullptr, &hwnd);
  
  _defaultCamera = new Camera(kScreenWidth, kScreenHeight);
  SetMainCamera(_defaultCamera);
}

void World::Cleanup() {}

World* World::CreateWorld(HWND hwnd, const std::wstring& title)
{
  World* newWorld = new World();
  newWorld->Initialize(hwnd, title);

  return nullptr;
}

void World::SetMainCamera(Camera* camera) {
  if (camera)
    mainCamera = camera;
  else
    mainCamera = _defaultCamera;
}

void World::FixCameraPosition(bool fixed) {
  bCameraMove = !fixed;
}

XMVECTOR World::ScreenToWorldPosition(XMVECTOR screenPos)
{


  return XMVECTOR();
}

void World::ProcessInput(float dt) {
  // _camera->LookAt({10, 0, 0, 1});
  // 'Q' 누르면 Down
  if (Input.IsKeyPress(Key::Q))
  {
    mainCamera->MoveDownUp(-dt * 6);
  }
  // 'E' 누르면 Up
  if (Input.IsKeyPress(Key::E))
  {
    mainCamera->MoveDownUp(dt * 6);
  }
  // 'A' 누르면 Left
  if (Input.IsKeyPress(Key::A))
  {
    mainCamera->MoveLeftRight(-dt * 6);
  }
  // 'D' 누르면 Right
  if (Input.IsKeyPress(Key::D))
  {
    mainCamera->MoveLeftRight(dt * 6);
  }
  // 'W' 누르면 Forward
  if (Input.IsKeyPress(Key::W))
  {
    mainCamera->MoveBackForward(dt * 6);
  }
  // 'S' 누르면 Backward
  if (Input.IsKeyPress(Key::S))
  {
    mainCamera->MoveBackForward(-dt * 6);
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

void World::FixedUpdate(float dt) {}

void World::PreUpdate(float dt) {}

void World::Update(float dt) {}

void World::PostUpdate(float dt) {}

void World::Render()
{
  if (!_currentLevel)
  {

    return;
  }

  for (GameObject* gameObject : _currentLevel->GetGameObjectList())
  {
    auto* transformComponent = gameObject->GetComponent<TransformComponent>();
    auto* meshComponent = gameObject->GetComponent<MeshComponent>();
    if (meshComponent)
    {
      const XMMATRIX& transform = gameObject->GetWorldTransform();
      MeshHandle meshHandle = meshComponent->GetHandle();

      _renderer->BeginDraw(meshHandle, transform);
      _renderer->DrawMesh(meshHandle);
    }
  }

}
