#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "Core/Camera/Camera.h"

class FixedCamera : public GameObject
{
  bool bMouseRightButtonPressed{false};
  Camera* cam{nullptr};

  XMVECTOR focus{};

public:
  FixedCamera(World* world);

  void SetCameraPosition(XMVECTOR position);
  void SetFocus(XMVECTOR focus);

  void DestroyCamera();

  void SetAsMainCamera();
  void UnsetAsMainCamera();

  // Default camera update logic
  virtual void Update(float dt) override;
  virtual void OnRender() override;
  float cameraPos[3] = {-30.f, 20.f, -30.f};
  float LookAt[3] = {3.f, 0.f, 3.f};
};
