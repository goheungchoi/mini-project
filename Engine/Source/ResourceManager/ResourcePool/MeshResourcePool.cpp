#include "ResourcePool.h"

#include "Shared/Serialize/model_generated.h"
#include <flatbuffers/flatbuffers.h>
#include <fstream>

namespace
{
struct Pools
{
  ResourcePool<TextureData>* texturePool;
  ResourcePool<MaterialData>* materialPool;
};

static ResourcePool<TextureData>* texturePool;
static ResourcePool<MaterialData>* materialPool;
}

template <>
Handle ResourcePool<MeshData>::LoadImpl(xUUID uuid, void* pUser)
{
  Pools* pools = (Pools*)pUser;
  ::texturePool = pools->texturePool;
  ::materialPool = pools->materialPool;

  fs::path path = GetResourcePath(uuid);

	std::ifstream ifs(path, std::ios::binary);
  std::vector<char> buffer((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());

	// const GameResource::Mesh* geoModel = GameResource::GetMesh(buffer.data());



	return Handle::kInvalidHandle;
}

