#pragma once

#include "Core/Common.h"

#include <Core/Handle/ResourceHandle.h>

struct ModelData {
  std::vector<MeshHandle> meshes;
  std::unordered_map<MeshHandle, MaterialHandle> materials;
  std::unordered_map<MeshHandle, TextureHandle> textures;
};
