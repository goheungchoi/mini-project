#pragma once
enum RenderType : uint8_t
{
  RenderTypeNone= 0,
  kOutline = 1<<0,
};
using RenderTypeFlags = uint8_t;