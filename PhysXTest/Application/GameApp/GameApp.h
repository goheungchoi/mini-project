#pragma once
#include "Core/Camera/Camera.h"
#include "GameEngine/GameEngine.h"
#include "GameFramework/GameObject/GameObject.h"
#include "Renderer/IRenderer.h"
struct PhyjixRay;
class IPhyjixEngine;
class IPhyjixWorld;
class RigidbodyComponent;

class GameApp : public GameEngine
{
  using Super = GameEngine;

public:
  GameApp() = default;
  ~GameApp() = default;

public:
  virtual void Initialize(UINT screenWidth, UINT screenHeight,
                          const std::wstring& title) override;
  virtual void Execute() override;
  virtual void Shutdown() override;

protected:
  virtual void FixedUpdate(float deltaTime) override;
  virtual void Update(float deltaTime) override;
  virtual void Render() override;

private:
  bool bCameraMove{false};
  Camera* _camera;
  DirectionalLight _mainLight;
  IRenderer* _renderer;
  Vector4 eye;
  Vector4 at;

  IPhyjixEngine* _phyjixEngine;
  IPhyjixWorld* _phyjixWorld;


  GameObject* testobject = nullptr;
  RigidbodyComponent* testrigidbody = nullptr;

  GameObject* testobject2 = nullptr;
  RigidbodyComponent* testrigidbody2 = nullptr;

  PhyjixRay* ray;
  Color debugcolor;

  bool _bCameraMove = false;
};
