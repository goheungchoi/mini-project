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
  ResourcePool<MeshData>* meshPool;
  ResourcePool<SkeletonData>* skeletonPool;
  ResourcePool<AnimationData>* animationPool;
};

static Pools* pools;

static ResourcePool<TextureData>* texturePool;
static ResourcePool<MaterialData>* materialPool;
static ResourcePool<MeshData>* meshPool;
static ResourcePool<SkeletonData>* skeletonPool;
static ResourcePool<AnimationData>* animationPool;
}

static void ProcessGeoMesh(ModelData& model, ModelNode& node,
                           const flatbuffers::String* geoMesh)
{
  Handle meshHandle = ::meshPool->Load(geoMesh->c_str(), ::pools);

	// Check if the mesh is successfully loaded
	if (::meshPool->IsValidHandle(meshHandle))
  {
		// Push the mesh handle
    model.meshes.insert(meshHandle);
    node.meshes.push_back(meshHandle);

		// Get the mesh's material
    const MeshData& meshData = ::meshPool->AccessResourceData(meshHandle);
		MaterialHandle matHandle = meshData.material;
		// Check if the mesh has a material
		if (::materialPool->IsValidHandle(matHandle))
    {
			// Add the material handle
      model.materials.insert(matHandle);

			// Get the material's textures
      const MaterialData& matData =
          ::materialPool->AccessResourceData(matHandle);

			// Albedo
      TextureHandle albedoTexture = matData.albedoTexture;
      if (::texturePool->IsValidHandle(albedoTexture))
      {
        model.textures.insert(albedoTexture);
      }

			// Metallic roughness
      TextureHandle metallicRoughnessTexture = matData.metallicRoughnessTexture;
      if (::texturePool->IsValidHandle(metallicRoughnessTexture))
      {
        model.textures.insert(metallicRoughnessTexture);
      }

			// Normal
      TextureHandle normalTexture = matData.normalTexture;
      if (::texturePool->IsValidHandle(normalTexture))
      {
        model.textures.insert(normalTexture);
      }

			// Occlusion
      TextureHandle occlusionTexture = matData.occlusionTexture;
      if (::texturePool->IsValidHandle(occlusionTexture))
      {
        model.textures.insert(occlusionTexture);
      }

			// Emissive
      TextureHandle emissiveTexutre = matData.emissiveTexture;
      if (::texturePool->IsValidHandle(emissiveTexutre))
      {
        model.textures.insert(emissiveTexutre);
      }
		}
	}
}

static void ProcessGeoNode(ModelData& model, 
													 const GameResource::GeometryNode* geoNode,
                           const GameResource::GeometryModel* geoModel)
{
	// Create a node
  ModelNode node{.name = geoNode->name()->c_str(),
                 .transform = Matrix((float*)geoNode->transform()),
                 .level = geoNode->level(),
                 .parent = geoNode->parent(),
                 .firstChild = geoNode->firstChild(),
                 .nextSibling = geoNode->nextSibling()};
  node.meshes.reserve(geoNode->meshes()->size());
	for (const auto* geoMesh : *geoNode->meshes())
  {
    ProcessGeoMesh(model, node, geoMesh);
	}
	// Added to the model
  model.nodes.emplace_back(std::move(node));
}

static void ProcessSkeleton(ModelData& model, const flatbuffers::String* flatSkeleton)
{
  Handle skeletonHandle = ::skeletonPool->Load(flatSkeleton->c_str(), nullptr);
  if (::skeletonPool->IsValidHandle(skeletonHandle))
  {
    model.skeleton = skeletonHandle;
	}
}

// TODO: Animation
static void ProcessAnimation(ModelData& model,
                             const flatbuffers::String* flatAnimation)
{
  Handle animationHandle =
      ::animationPool->Load(flatAnimation->c_str(), nullptr);
	if (::animationPool->IsValidHandle(animationHandle)) {
    model.animations.insert(animationHandle);
	}
}

template<>
Handle ResourcePool<ModelData>::LoadImpl(xUUID uuid, void* pUser)
{
  ::pools = (Pools*)pUser;
  ::texturePool = pools->texturePool;
  ::materialPool = pools->materialPool;
  ::meshPool = pools->meshPool;
  ::skeletonPool = pools->skeletonPool;
  ::animationPool = pools->animationPool;

  fs::path path = GetResourcePath(uuid);

  std::ifstream ifs(path, std::ios::binary);

	if (!ifs.is_open())
  {
    throw std::exception("Failed to open a file.");
	}

  std::vector<char> buffer((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());

  const GameResource::GeometryModel* geoModel =
      GameResource::GetGeometryModel(buffer.data());

	ModelData model;
  model.nodes.reserve(geoModel->nodes()->size());
	for (const auto* geoNode : *geoModel->nodes())
  {
    ProcessGeoNode(model, geoNode, geoModel);
	}

	const auto* flatSkeleton = geoModel->skeleton();
  if (flatSkeleton)
  {
    ProcessSkeleton(model, flatSkeleton);
	}

	const auto* flatAnimations = geoModel->animations();
  if (flatAnimations)
  {
    for (const auto* flatAnimation : *flatAnimations)
    {
      ProcessAnimation(model, flatAnimation);
		}
	}

	// Claim the handle and map the UUID.
	Handle handle = _handleTable.ClaimHandle(std::move(model),
                                                (uint16_t)ResourceType::kModel);
  _uuidMap[uuid] = handle.index;
  _handleUUIDMap[handle] = uuid;
  return handle;
}


template <>
void ResourcePool<ModelData>::UnloadImpl(Handle& handle, void* pReserved)
{
  ::pools = (Pools*)pReserved;
  ::texturePool = pools->texturePool;
  ::materialPool = pools->materialPool;
  ::meshPool = pools->meshPool;
  ::skeletonPool = pools->skeletonPool;
  ::animationPool = pools->animationPool;

  ModelData& model = _handleTable[handle].value();

  for (MeshHandle mesh : model.meshes)
  {
    if (::meshPool->IsValidHandle(mesh))
    {
      meshPool->Unload(mesh, ::pools);
    }
  }

  for (MaterialHandle mat : model.materials)
  {
    if (::materialPool->IsValidHandle(mat))
    {
      materialPool->Unload(mat, ::pools);
    }
  }

  for (TextureHandle texture : model.textures)
  {
    if (::texturePool->IsValidHandle(texture))
    {
      texturePool->Unload(texture, ::pools);
    }
  }

  for (AnimationHandle anim : model.animations)
  {
    if (::texturePool->IsValidHandle(anim))
    {
      texturePool->Unload(anim, nullptr);
    }
  }

  if (::skeletonPool->IsValidHandle(model.skeleton))
  {
    ::skeletonPool->Unload(model.skeleton, nullptr);
  }
}
