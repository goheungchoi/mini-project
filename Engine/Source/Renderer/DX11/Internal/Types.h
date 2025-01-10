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

//그림자는 기본적으로 지원.
enum RenderPassType : uint8_t
{
  None = 0,       
  OpaquePass = 1 << 0,      // 0000 0001
  TransparentPass = 1 << 1, // 0000 0010
  LightPass = 1 << 2,       // 0000 0100
};
using RenderPassFlags = uint8_t;
