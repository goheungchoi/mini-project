#pragma once

#include "GameFramework/Common.h"

#include "Renderer/IRenderer.h"

class RenderSystem
{
  IRenderer* _renderer;

public:
  RenderSystem(IRenderer* renderer) : _renderer(renderer) {}

  void Execute(const std::list<class GameObject*>& gameObjects);
};
