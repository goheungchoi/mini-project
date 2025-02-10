#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "Core/Camera/Camera.h"

class FixedCamera : public GameObject
{
  bool bMouseRightButtonPressed{false};
  Camera* cam{nullptr};

  XMVECTOR focus{4.f,-1.5f,4.f};

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
  float cameraPos[2] = {-21.f, -21.f};
  float cameraY = 20.f;
  float LookAt[3] = {4.f, -1.5f, 4.f};
};
