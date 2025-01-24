#pragma once

#include "GameEngine/GameEngine.h"

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
  virtual void ProcessInput(float dt) override;
  virtual void Update(float deltaTime) override;
  virtual void Render() override;

  virtual bool OnActivated() override;
  virtual bool OnDeactivated() override;
  virtual bool OnSuspending() override;
  virtual bool OnResuming() override;
  virtual bool OnWindowResized() override;

private:
  virtual void Run() override;

private:
  class World* _world;
};
