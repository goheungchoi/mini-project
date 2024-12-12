#pragma once

#include "../../Engine/Source/Engine/GameEngine.h"

class GameApp : public GameEngine
{
	using Super = GameEngine;

public:
  GameApp() = default;
  ~GameApp() = default;

public:
  virtual void Initialize() override;

protected:
  virtual void FixedUpdate(float deltaTime) override;
  virtual void Update(float deltaTime) override;
  virtual void Render() override;

};
