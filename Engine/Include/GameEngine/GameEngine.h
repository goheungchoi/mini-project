#pragma once

#include "Core/Common.h"

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
  virtual void FixedUpdate(float deltaTime) = 0;
  virtual void Update(float deltaTime) = 0;
  virtual void Render() = 0;

  virtual bool OnActivated();
  virtual bool OnDeactivated();
  virtual bool OnSuspending();
  virtual bool OnResuming();
  virtual bool OnWindowResized();

private:
  // Main Loop
  void Run();

protected:
  HWND _hwnd{};
};