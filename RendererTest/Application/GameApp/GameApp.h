#pragma once

#include "GameEngine/GameEngine.h"
#include "../../Engine/Source/Renderer/IRenderer.h"
#include "Core/Camera/Camera.h"
class GameApp : public GameEngine
{
	using Super = GameEngine;

public:
  GameApp() = default;
  ~GameApp() = default;

public:
  virtual void Initialize() override;
  virtual void Execute() override;
  virtual void Shutdown() override;

protected:
  virtual void FixedUpdate(float deltaTime) override;
  virtual void Update(float deltaTime) override;
  virtual void Render() override;

private:
  Camera* _camera;
  Light::DirectionalLight _mainLight;
  IRenderer* _renderer;
  Vector4 eye;
  Vector4 at;
};
