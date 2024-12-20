#pragma once

#include "Core/Common.h"

#include "Core/Handle/ResourceHandle.h"

// NOTE:
#include "directxtk/SimpleMath.h"

using namespace DirectX::SimpleMath;

struct Vertex
{
  Vector4 position;
  Vector3 normal;
  Vector3 tangent;
  Vector3 bitangent;
  Vector2 uv;
  Vector3 color;
};

using Index = uint32_t;

struct MeshData {
  std::vector<Vertex> vertices;
  std::vector<Index> indices;
  uint32_t stride;
  uint32_t offset;

	MaterialHandle material;
};
