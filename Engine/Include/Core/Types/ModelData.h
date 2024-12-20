#pragma once

#include "Core/Common.h"

#include <Core/Handle/ResourceHandle.h>

struct ModelNode
{
  std::string name;
	// Matrix transformation;

  uint32_t parent;
  uint32_t firstChild;
  uint32_t nextSibling;
  uint32_t level;
	
	std::vector<MeshHandle> meshes;
};

struct ModelData {
  uint32_t rootNode;
  std::vector<ModelNode> nodes;
  std::vector<MeshHandle> meshes;
  std::vector<MaterialHandle> materials;
  std::vector<TextureHandle> textures;
};
