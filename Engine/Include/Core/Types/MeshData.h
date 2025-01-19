#pragma once

#include "Core/Common.h"

#include "Core/Handle/ResourceHandle.h"

#include "BoneData.h"

struct Vertex
{
  Vector4 position;
  Vector3 normal;
  Vector3 tangent;
  Vector3 bitangent;
  Vector2 uv;
  Vector4 color;
};

using Index = uint32_t;

struct MeshData {
  std::string name;

  MathUtil::AABB boundingBox;

  std::vector<Vertex> vertices;
  std::vector<Index> indices;
  uint32_t stride;
  uint32_t offset;

	std::vector<Bone> bones;
	std::vector<BoneId> boneIds;
  std::vector<BoneWeight> boneWeights;

	MaterialHandle material;
};
