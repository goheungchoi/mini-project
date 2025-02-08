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

  if (INPUT.IsKeyPress(Key::Q) || INPUT.IsKeyDown(Key::Q))
  {
    cam->MoveDownUp(-dt);
  }
  if (INPUT.IsKeyPress(Key::E) || INPUT.IsKeyDown(Key::E))
  {
    cam->MoveDownUp(dt);
  }
  if (INPUT.IsKeyPress(Key::A) || INPUT.IsKeyDown(Key::A))
  {
    cam->MoveLeftRight(-dt);
  }
  if (INPUT.IsKeyPress(Key::D) || INPUT.IsKeyDown(Key::D))
  {
    cam->MoveLeftRight(dt);
  }
  if (INPUT.IsKeyPress(Key::W) || INPUT.IsKeyDown(Key::W))
  {
    cam->MoveBackForward(dt);
  }
  if (INPUT.IsKeyPress(Key::S) || INPUT.IsKeyDown(Key::S))
  {
    cam->MoveBackForward(-dt);
  }
  if (INPUT.IsKeyPress(Key::OemMinus) || INPUT.IsKeyDown(Key::OemMinus))
  {
    cam->AddMoveSpeed(-10);
  }
  if (INPUT.IsKeyPress(Key::OemPlus) || INPUT.IsKeyDown(Key::OemPlus))
  {
    cam->AddMoveSpeed(+10);
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