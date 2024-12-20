#pragma once
#include "Core/Common.h"
#include "Common.h"
#include "Core/Handle/ResourceHandle.h"
struct MeshBuffer;
struct Sampler;

struct ResourceStorage
{
  std::unordered_map<MeshHandle, MeshBuffer*> meshMap;
};