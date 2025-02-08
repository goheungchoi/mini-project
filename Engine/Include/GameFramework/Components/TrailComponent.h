#pragma once
#include "ComponentBase.h"
#include "Core/Types/Trail.h"
class TrailComponent : public ComponentBase
{
public:
  TrailComponent(class GameObject* owner);
  ~TrailComponent();
  void Update(float dt, DirectX::XMVECTOR currPos, XMVECTOR eye);
  void SetDuration(float duration);
  void SetWidth(float width);
  void SetDirection(DirectX::XMVECTOR direction);
  void SetColor(Color color);

public:
  bool isVisible{true};
  Trail* trail = nullptr;
};