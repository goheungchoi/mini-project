#pragma once

#include "Core/Common.h"

#include "Core/Handle/ResourceHandle.h"

struct ModelNode
{
  std::string name;
	Matrix transform;

  int level;
  int parent;
  int firstChild;
  int nextSibling;
	
	std::vector<MeshHandle> meshes;
};

struct ModelData {
  std::string name;

  std::vector<ModelNode> nodes;
  std::unordered_set<MeshHandle> meshes;
  std::unordered_set<MaterialHandle> materials;
  std::unordered_set<TextureHandle> textures;

	// Animations
  std::unordered_set<AnimationHandle> animations;

	// Skeleton
  SkeletonHandle skeleton;
};
