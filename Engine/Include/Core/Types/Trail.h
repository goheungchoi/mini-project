#pragma once
#include "Core/Common.h"
#include "directxtk/SimpleMath.h"
using namespace DirectX::SimpleMath;
class TrailResource;
class Device;
struct TrailPoint
{
  Vector3 position;
  float age;
};
class Trail
{
private:
  float _maxDuration{2.f};
  float _width{1.f};
  DirectX::XMVECTOR _direction;

public:
  Trail(Device* device);
  ~Trail();

public:
  void UpdateTrail(float dt, DirectX::XMVECTOR currPos, DirectX::XMVECTOR eye);
  void SetMaxDuration(float duration) { _maxDuration = duration; }
  void SetWidth(float width) { _width = width; }
  void SetDirection(DirectX::XMVECTOR direction) { _direction = direction; }

public:
  TrailResource* trailResource{nullptr};
  Vector4 position;

private:
  std::deque<TrailPoint> _trailPoints;
};