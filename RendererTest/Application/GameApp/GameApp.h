#pragma once

#include "../../Engine/Source/Engine/GameEngine.h"
#include "../../Engine/Source/Renderer/IRenderer.h"
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
  IRenderer* _renderer;
};
