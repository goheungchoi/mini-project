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
	// Get AABB
  mesh.boundingBox.min = {geoMesh->aabb()->min().x(),
                          geoMesh->aabb()->min().y(),
                          geoMesh->aabb()->min().z(), 1.f};
  mesh.boundingBox.max = {geoMesh->aabb()->max().x(),
                          geoMesh->aabb()->max().y(),
                          geoMesh->aabb()->max().z(), 1.f};
	
	// Get vertices
  const auto* flatVertices = geoMesh->vertices();
  if (flatVertices)
  {
    mesh.vertices.reserve(flatVertices->size());
    for (const auto& flatVertex : *flatVertices)
    {
      Vertex vertex;
      vertex.position = {
          flatVertex->position().x(), flatVertex->position().y(),
          flatVertex->position().z(), flatVertex->position().w()};
      vertex.normal = {flatVertex->normal().x(), flatVertex->normal().y(),
                       flatVertex->normal().z()};
      vertex.tangent = {flatVertex->tangent().x(), flatVertex->tangent().y(),
                        flatVertex->tangent().z()};
      vertex.bitangent = {flatVertex->bitangent().x(),
                          flatVertex->bitangent().y(),
                          flatVertex->bitangent().z()};
      vertex.uv = {flatVertex->texcoord().u(), flatVertex->texcoord().v()};
      vertex.color = {flatVertex->color().x(), flatVertex->color().y(),
                      flatVertex->color().z()};

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

	// Get bones
  const auto* flatBones = geoMesh->bones();
  if (flatBones)
  {
    mesh.bones.reserve(flatBones->size());
    for (const auto& flatBone : *flatBones)
    {
      Bone bone{.id = flatBone->id(),
                .name = flatBone->name()->c_str(),
                .inverseBindMatrix = XMMATRIX((float*)flatBone->offset())};
      mesh.bones.push_back(bone);
		}
	}

	// Get bone influences
  const auto* flatBoneWeights = geoMesh->vertexBoneWeights();
  if (flatBoneWeights)
  {
    mesh.boneIds.resize(flatBoneWeights->size());
    mesh.boneWeights.resize(flatBoneWeights->size());
    for (size_t i = 0; i < flatBoneWeights->size(); ++i)
    {
      mesh.boneIds[i] = flatBoneWeights->operator[](i)->boneId();
      mesh.boneWeights[i] = flatBoneWeights->operator[](i)->weight();
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

	Handle handle =
      _handleTable.ClaimHandle(std::move(mesh), (uint16_t)ResourceType::kMesh);
  _uuidMap[uuid] = handle.index;
  return handle;
}

