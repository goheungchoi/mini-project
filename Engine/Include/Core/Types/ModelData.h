#pragma once

#include "Core/Common.h"

#include <Core/Handle/ResourceHandle.h>

struct ModelNode
{
  std::string name;
	// Matrix transformation;

  uint32_t level;
  uint32_t parent;
  uint32_t firstChild;
  uint32_t nextSibling;
	
	std::vector<MeshHandle> meshes;
};

struct ModelData {
  std::string name;

  std::vector<ModelNode> nodes;
  std::unordered_set<MeshHandle> meshes;
  std::unordered_set<MaterialHandle> materials;
  std::unordered_set<TextureHandle> textures;
};
