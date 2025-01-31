#include "CameraObject.h"

#include "GameFramework/World/World.h"

#include "Core/Input/InputSystem.h"

void CameraObject::InitCamera(uint32_t width, uint32_t height, float vfov,
                              float nearPlane, float farPlane)
{
  cam = new Camera(width, height, vfov, nearPlane, farPlane);
}

void CameraObject::DestroyCamera()
{
  UnsetAsMainCamera();
  delete cam;
  cam = nullptr;
}

void CameraObject::SetAsMainCamera()
{
  GetWorld()->SetMainCamera(cam);
}

void CameraObject::UnsetAsMainCamera() {
  GetWorld()->SetMainCamera(nullptr);
}

void CameraObject::Update(float dt)
{
  if (!cam)
    return;

  if (INPUT.IsKeyPress(Key::Q))
  {
    cam->MoveDownUp(-dt);
  }
  if (INPUT.IsKeyPress(Key::E))
  {
    cam->MoveDownUp(dt);
  }
  if (INPUT.IsKeyPress(Key::A))
  {
    cam->MoveLeftRight(-dt);
  }
  if (INPUT.IsKeyPress(Key::D))
  {
    cam->MoveLeftRight(dt);
  }
  if (INPUT.IsKeyPress(Key::W))
  {
    cam->MoveBackForward(dt);
  }
  if (INPUT.IsKeyPress(Key::S))
  {
    cam->MoveBackForward(-dt);
  }
  if (INPUT.IsKeyPress(Key::OemMinus))
  {
    cam->AddMoveSpeed(-0.3);
  }
  if (INPUT.IsKeyPress(Key::OemPlus))
  {
    cam->AddMoveSpeed(+0.3);
  }

  if (INPUT.IsKeyDown(MouseState::RB))
  {
    bMouseRightButtonPressed = true;
    ShowCursor(false); 
  }
  if (INPUT.IsKeyUp(MouseState::RB))
  {
    bMouseRightButtonPressed = false;
    ShowCursor(true);
  }

  if (bMouseRightButtonPressed)
  {
    Vector2 mouseDelta = INPUT.GetMouseDelta();
    float x = -mouseDelta.x;
    float y = -mouseDelta.y;

    cam->RotateAroundXAxis(y);
    cam->RotateAroundYAxis(x);
  }
}