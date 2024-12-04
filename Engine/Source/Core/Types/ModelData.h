#pragma once

#include "Core/Common.h"

struct ModelData {
  std::vector<Handle> meshes;
  std::vector<Handle, Handle> materials;
  std::unordered_map<Handle, Handle> textures;
};
