#pragma once

#include "Core/Common.h"

enum class ShaderType : uint8_t
{
  kUnknown = 0,

  kVertex,
  kGeometry,
  kHull,
  kTessellator,
  kDomain,
	kPixel,

  kCompute,

  kRayGen,
  kRayIntersect,
  kRayAnyHit,
  kRayClosestHit,
  kRayMiss,
  kRayCallable,

  kMesh,
  kTask,

  kInvalid = (std::numeric_limits<uint8_t>::max)()
};

enum class ShaderDefineValueType : uint8_t
{
  kNone,    // 0 bytes
  kBool,    // 1 bytes
  kInteger, // 4 bytes
  kFloat,   // 4 bytes
};

union ShaderDefineValue {
  char vNone;
  bool vBool;
  int vInt;
  float vFloat;
};

struct ShaderDefine
{
  std::string define;
  ShaderDefineValueType type;
  ShaderDefineValue value;
};

struct ShaderOption
{
  std::string name;
  std::string define;
  ShaderDefineValueType type;
  std::vector<ShaderDefineValue> values;
};

struct ShaderData
{
  std::string name;
  std::string path;
  ShaderType type;
  std::vector<ShaderDefine> defines;

  std::vector<uint8_t> data;
};
