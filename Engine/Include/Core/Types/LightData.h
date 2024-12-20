#pragma once

// NOTO:
#include "directxtk/SimpleMath.h"

using namespace DirectX::SimpleMath;

enum class LightType : uint32_t
{
  kUndefined = 0,
  kDirectional,
  kPoint,
  kSpot
};

struct LightData
{
  Vector4 components; // [x,y,z,1] - position, [x,y,z,0] - direction
  Vector4 radiance;   // r, g, b, intensity

  float spotAngle;
  float constantAttenuation;
  float linearAttenuation;
  float quadraticAttenuation;

  LightType type;
  bool enabled;
};
