#include "Trail.h"
#include "Renderer/DX11/Internal/Device.h"
#include "Renderer/DX11/Internal/Resources/TrailResrouce.h"
Trail::Trail(Device* device) 
{
  trailResource = new TrailResource(device);
}

Trail::~Trail() 
{
  SAFE_RELEASE(trailResource);
}

void Trail::UpdateTrail(float dt, DirectX::XMVECTOR currPos,
                        DirectX::XMVECTOR eye)
{
  position = currPos;
  _trailPoints.push_back({currPos, 0.f});
  for (auto& point : _trailPoints)
  {
    point.age += dt;
  }
  while (!_trailPoints.empty() && _trailPoints.front().age > 1.f)
  {
    _trailPoints.pop_front();
  }

 trailResource->UpdateTrailResource(_trailPoints,_maxDuration,_width,_direction,eye);
}
