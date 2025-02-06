#pragma once

#include "Core/Math/MathUtils.h"

struct DirectionalLight
{
  Vector4 direction = Vector4(0.804f,-0.864f,0.615f,0.f);
  Vector4 radiance = {1.f, 0.9569f, 0.8392f, 1.f};
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
