#include "FixedCameraObject.h"

#include "GameFramework/World/World.h"

#include "Core/Input/InputSystem.h"

FixedCameraObject::FixedCameraObject(World* world) : GameObject(world)
{
  cam = new Camera(kScreenWidth, kScreenHeight, XM_PIDIV4, 0.01f, 1000.f);
}

void FixedCameraObject::SetCameraPosition(XMVECTOR position)
{
  cam->SetPosition(position);
}

void FixedCameraObject::SetFocus(XMVECTOR focus)
{
  this->focus = focus;
  cam->LookAt(focus);
}

void FixedCameraObject::DestroyCamera()
{
  UnsetAsMainCamera();
  delete cam;
  cam = nullptr;
}

void FixedCameraObject::SetAsMainCamera()
{
  GetWorld()->SetMainCamera(cam);
}

void FixedCameraObject::UnsetAsMainCamera()
{
  GetWorld()->SetMainCamera(nullptr);
}

void FixedCameraObject::Update(float dt)
{
  static bool set{true};
  if (INPUT.IsKeyPress(Key::R))
  {
    set = !set;
    if (set)
    {
      SetAsMainCamera();
    }
    else
    {
      UnsetAsMainCamera();
    }
  }

  // XMVECTOR toCamera = XMVectorSubtract(cam->GetPosition(), this->focus);
  // float rotationAngle = cam->GetRotationSpeed() *
  //                       XMConvertToRadians(30.0f * dt); // 5 degrees per
  //                       update

  // if (Input.IsKeyPress(Key::Q))
  // {
  //   // Rotate left: Create a quaternion for rotating around the Y-axis (up)
  //   XMVECTOR rotation = XMQuaternionRotationAxis(Camera::UP, rotationAngle);

  //   // Apply the rotation to the vector from the focus to the camera
  //   toCamera = XMVector3Rotate(toCamera, rotation);

  //   // Update the camera position
  //   cam->SetPosition(XMVectorAdd(this->focus, toCamera));

  //   // Adjust the yaw to reflect the new rotation
  //   cam->RotateAroundYAxis(-XMConvertToDegrees(rotationAngle)); // Left
  //   rotation
  // }
  // if (Input.IsKeyPress(Key::E))
  // {
  //   // Rotate right: Create a quaternion for rotating around the Y-axis (up)
  //   XMVECTOR rotation = XMQuaternionRotationAxis(Camera::UP, -rotationAngle);

  //   // Apply the rotation to the vector from the focus to the camera
  //   toCamera = XMVector3Rotate(toCamera, rotation);

  //   // Update the camera position
  //   cam->SetPosition(XMVectorAdd(this->focus, toCamera));

  //   // Adjust the yaw to reflect the new rotation
  //   cam->RotateAroundYAxis(
  //       XMConvertToDegrees(rotationAngle)); // Right rotation
  // }

  // cam->LookAt(focus);
}