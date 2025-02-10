#include "FixedCamera.h"

#include "GameFramework/World/World.h"

#include "Core/Input/InputSystem.h"
#ifdef _DEBUG
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#endif

FixedCamera::FixedCamera(World* world) : GameObject(world)
{
  float angle = XM_PIDIV2 / 9*2;
  cam = new Camera(kScreenWidth, kScreenHeight, angle, 10.f, 100.f);
}

void FixedCamera::SetCameraPosition(XMVECTOR position)
{
  cam->SetPosition(position);
}

void FixedCamera::SetFocus(XMVECTOR focus)
{
  this->focus = focus;
  cam->LookAt(focus);
}

void FixedCamera::DestroyCamera()
{
  UnsetAsMainCamera();
  delete cam;
  cam = nullptr;
}

void FixedCamera::SetAsMainCamera()
{
  GetWorld()->SetMainCamera(cam);
}

void FixedCamera::UnsetAsMainCamera()
{
  GetWorld()->SetMainCamera(nullptr);
}

void FixedCamera::Update(float dt)
{
  static bool set{true};
  /*if (INPUT.IsKeyPress(Key::R))
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
  }*/


   //XMVECTOR toCamera = XMVectorSubtract(cam->GetPosition(), this->focus);
   //float rotationAngle = cam->GetRotationSpeed() *
   //                      XMConvertToRadians(60.0f * dt); // 10 degrees per
   //                      //update

   //if (INPUT.IsKeyPress(Key::Q) || INPUT.IsKeyDown(Key::Q))
   //{
   //  // Rotate left: Create a quaternion for rotating around the Y-axis (up)
   //  XMVECTOR rotation = XMQuaternionRotationAxis(Camera::UP, rotationAngle);

   //  // Apply the rotation to the vector from the focus to the camera
   //  toCamera = XMVector3Rotate(toCamera, rotation);

   //  // Update the camera position
   //  cam->SetPosition(XMVectorAdd(this->focus, toCamera));

   //  // Adjust the yaw to reflect the new rotation
   //  cam->RotateAroundYAxis(-XMConvertToDegrees(rotationAngle)); // Left
   //  //rotation
   //}
   //if (INPUT.IsKeyPress(Key::E) || INPUT.IsKeyDown(Key::E))
   //{
   //  // Rotate right: Create a quaternion for rotating around the Y-axis (up)
   //  XMVECTOR rotation = XMQuaternionRotationAxis(Camera::UP, -rotationAngle);

   //  // Apply the rotation to the vector from the focus to the camera
   //  toCamera = XMVector3Rotate(toCamera, rotation);

   //  // Update the camera position
   //  cam->SetPosition(XMVectorAdd(this->focus, toCamera));

   //  // Adjust the yaw to reflect the new rotation
   //  cam->RotateAroundYAxis(
   //      XMConvertToDegrees(rotationAngle)); // Right rotation
   //}

   //cam->LookAt(focus);
}

void FixedCamera::OnRender() 
{
#ifdef _DEBUG
  //if (ImGui::Begin("FixedCamera"))
  //{
  //  ImGui::SliderFloat2("fixedCameraPos", cameraPos, -50.f, 0.f);
  //  ImGui::SliderFloat("fixedCameraPosY", &cameraY, 0, 50.f);
  //  ImGui::SliderFloat3("fixedCameralookat", LookAt, -10.f, 10.f);
  //}
  //ImGui::End();
  //XMVECTOR cp = {cameraPos[0], cameraY, cameraPos[1]};
  //XMVECTOR la = {LookAt[0], LookAt[1], LookAt[2]};
  //cam->SetPosition(cp);
  //cam->LookAt(la);
#endif // !_DEBUG
}
