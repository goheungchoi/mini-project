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

  kInvalid = std::numeric_limits<uint8_t>::max()
};

enum class ShaderDefineValueType : uint8_t
{
  kNone,    // 0 bytes
  kBool,    // 1 bytes
  kInteger, // 4 bytes
  kFloat,   // 4 bytes
};

struct ShaderDefine
{
  ShaderDefineValueType type;
  std::string define;
  union Value {
    char vNone[0];
    bool vBool;
    int vInt;
    float vFloat;
	} value;
};

struct ShaderData {
  std::string name;
  std::string path;
  std::vector<ShaderDefine> defines;
	ShaderType type;

	std::vector<uint8_t> data;
};
