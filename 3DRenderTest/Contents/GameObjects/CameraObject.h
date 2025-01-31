#pragma once

#include "GameFramework/GameObject/GameObject.h"

#include "Core/Camera/Camera.h"

class CameraObject : public GameObject
{
  bool bMouseRightButtonPressed{false};
  Camera* cam{nullptr};

public:
  CameraObject(World* world) : GameObject(world) {}

  void InitCamera(uint32_t width, uint32_t height,
                  float vfov = MathUtil::kHalfPi_f, float nearPlane = 0.01f,
                  float farPlane = 10000.f);


	void DestroyCamera();
	
	void SetAsMainCamera();
	void UnsetAsMainCamera();

  // Default camera update logic
  virtual void Update(float dt) override;
};
