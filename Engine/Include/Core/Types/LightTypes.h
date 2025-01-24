#pragma once

#include "Core/Math/MathUtils.h"

struct DirectionalLight
{
  Vector4 direction = Vector4(1.f,-1.f,1.f,0.f);
  Vector4 radiance;
};

struct PointLight
{
  Vector4 position;
  Vector4 direction;
  Vector4 radiance;
};

struct SpotLight
{
  Vector4 position;
  Vector4 direction;
  Vector4 radiance;
};
