#pragma once
#include "Core/Common.h"
#include "directxtk/SimpleMath.h"
using namespace DirectX::SimpleMath;
namespace Light
{
  struct DirectionalLight
  {
    Vector4 direction;
    Vector4 color;
    Vector4 intensity;
  };
  struct PointLight
  {
    Vector4 position;
    Vector4 direction;
    Vector4 color;
    Vector4 intensity;
  };
}

enum RenderPassType : uint8_t
{
  None = 0,       
  kOpaquePass = 1 << 0,      // 0000 0001
  kTransparentPass = 1 << 1, // 0000 0010
  kShadowPass = 1 << 2,       // 0000 0100
  kLightPass = 1 << 3,       // 0000 0100
};
using RenderPassFlags = uint8_t;