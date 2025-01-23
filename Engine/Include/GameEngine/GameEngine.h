#pragma once

#include "Core/Common.h"

constexpr float kFixedRate{1.f / 60.f};

class GameEngine
{
public:
  GameEngine() = default;
  ~GameEngine() = default;

public:
  virtual void Initialize(UINT screenWidth, UINT screenHeight,
                          const std::wstring& title);
  virtual void Execute();
  virtual void Shutdown();

protected:
  virtual void ProcessInput(float dt) = 0;
  virtual void FixedUpdate(float dt) = 0;
  virtual void Update(float dt) = 0;
  virtual void Render() = 0;

  virtual bool OnActivated();
  virtual bool OnDeactivated();
  virtual bool OnSuspending();
  virtual bool OnResuming();
  virtual bool OnWindowResized();

private:
  // Main Loop
  virtual void Run();

protected:
  HWND _hwnd{};
};