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
  // 'Q' ������ Down
  if (Input.IsKeyPress(Key::Q))
  {
    mainCamera->MoveDownUp(-dt * 6);
  }
  // 'E' ������ Up
  if (Input.IsKeyPress(Key::E))
  {
    mainCamera->MoveDownUp(dt * 6);
  }
  // 'A' ������ Left
  if (Input.IsKeyPress(Key::A))
  {
    mainCamera->MoveLeftRight(-dt * 6);
  }
  // 'D' ������ Right
  if (Input.IsKeyPress(Key::D))
  {
    mainCamera->MoveLeftRight(dt * 6);
  }
  // 'W' ������ Forward
  if (Input.IsKeyPress(Key::W))
  {
    mainCamera->MoveBackForward(dt * 6);
  }
  // 'S' ������ Backward
  if (Input.IsKeyPress(Key::S))
  {
    mainCamera->MoveBackForward(-dt * 6);
  }

  if (Input.IsKeyDown(MouseState::RB))
  {
    bCameraMove = !bCameraMove; // camera bool���� �ݴ�� ��.
    ShowCursor(!bCameraMove);   // Ŀ���� �� ����
  }
  if (Input.IsKeyUp(MouseState::RB))
  {
    bCameraMove = !bCameraMove; // camera bool���� �ݴ�� ��.
    ShowCursor(!bCameraMove);   // Ŀ���� ����
  }

  // ���콺 ȸ��
  if (bCameraMove)
  {
    // ���콺 �̵��� ��������
    Vector2 mouseDelta = Input.GetMouseDelta();
    float x = -mouseDelta.x * mainCamera->GetRotationSpeed();
    float y = -mouseDelta.y * mainCamera->GetRotationSpeed();

    if ((Input.GetCurrMouseState().x != Input.GetPrevMouseState().x) ||
        (Input.GetCurrMouseState().y != Input.GetPrevMouseState().y))
    {
      // ���콺�� Y ������ �����̸� X �� ȸ��
      mainCamera->RotateAroundXAxis(y);
      // ���콺�� X ������ �����̸� Y �� ȸ��
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
