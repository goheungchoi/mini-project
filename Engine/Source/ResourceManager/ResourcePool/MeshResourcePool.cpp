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

static Pools* pools;

static ResourcePool<TextureData>* texturePool;
static ResourcePool<MaterialData>* materialPool;
} // namespace

static void ProcessMesh(MeshData& mesh, const GameResource::Mesh* geoMesh) {
	// Get vertices
  const auto* flatVertices = geoMesh->vertices();
  if (flatVertices)
  {
    mesh.vertices.reserve(flatVertices->size());
    for (const auto& flatVertex : *flatVertices)
    {
      Vertex vertex;
      vertex.position = {
          flatVertex->position()->x(), flatVertex->position()->y(),
          flatVertex->position()->z(), flatVertex->position()->w()};
      vertex.normal = {flatVertex->normal()->x(), flatVertex->normal()->y(),
                       flatVertex->normal()->z()};
      vertex.tangent = {flatVertex->tangent()->x(), flatVertex->tangent()->y(),
                        flatVertex->tangent()->z()};
      vertex.bitangent = {flatVertex->bitangent()->x(),
                          flatVertex->bitangent()->y(),
                          flatVertex->bitangent()->z()};
      vertex.uv = {flatVertex->texcoord()->u(), flatVertex->texcoord()->v()};
      vertex.color = {flatVertex->color()->x(), flatVertex->color()->y(),
                      flatVertex->color()->z()};

      mesh.vertices.push_back(vertex);
    }
  }

	// Get indices
  const auto* flatIndices = geoMesh->indices();
  if (flatIndices)
  {
    mesh.indices.reserve(flatIndices->size());
    for (const auto& index : *flatIndices)
    {
      mesh.indices.push_back(index);
    }
  }

	// Load the material
  Handle matHandle =
      ::materialPool->Load(geoMesh->material()->c_str(), ::pools);
  if (::materialPool->IsValidHandle(matHandle))
  {
    mesh.material = matHandle;
	}
}

template<>
Handle ResourcePool<MeshData>::LoadImpl(xUUID uuid, void* pUser)
{
  ::pools = (Pools*)pUser;
  ::texturePool = pools->texturePool;
  ::materialPool = pools->materialPool;

  fs::path path = GetResourcePath(uuid);

	std::ifstream ifs(path, std::ios::binary);
  std::vector<char> buffer((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());

	const GameResource::Mesh* geoMesh = flatbuffers::GetRoot<GameResource::Mesh>(buffer.data());

	MeshData mesh;
  ProcessMesh(mesh, geoMesh);

	return _handleTable.ClaimHandle(std::move(mesh),
                                  (uint16_t)ResourceType::kMesh);
}

