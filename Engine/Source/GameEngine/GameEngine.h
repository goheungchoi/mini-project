#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>

class GameEngine
{
public:
  GameEngine() = default;
  ~GameEngine() = default;

public:
  virtual void Initialize();
  virtual void Execute();
  virtual void Shutdown();

protected:
  virtual void FixedUpdate(float deltaTime) = 0;
  virtual void Update(float deltaTime) = 0;
  virtual void Render() = 0;

private:
  // Main Loop
  void Run();

protected:
  HWND _hwnd{};
};