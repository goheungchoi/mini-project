#pragma once
#include "Core/Common.h"

enum RenderPassType : uint8_t
{
  None = 0,       
  Opaque = 1 << 0,      // 0000 0001
  Transparent = 1 << 1, // 0000 0010
  Light = 1 << 2,       // 0000 0100
  Shadow = 1 << 3,      // 0000 1000
};

using RenderPassFlags = uint8_t;
