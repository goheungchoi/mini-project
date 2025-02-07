#pragma once
#include "ComponentBase.h"
#include "Core/Types/Trail.h"
class TrailComponent : public ComponentBase
{
public:
  TrailComponent(class GameObject* owner);
  ~TrailComponent();
  void Update(float dt);

private:
  Trail* trail = nullptr;
};