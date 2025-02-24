#include "ResourcePool.h"

#include "Shared/Serialize/model_generated.h"
#include <flatbuffers/flatbuffers.h>
#include <fstream>

namespace
{
struct Pools
{
  ResourcePool<TextureData>* texturePool;
};

static Pools* pools;

static ResourcePool<TextureData>* texturePool;
} // namespace

static void ProcessMaterial(MaterialData& mat,
                            const GameResource::Material* geoMat)
{
  mat.name = geoMat->name()->str();

  mat.albedoFactor = {geoMat->albedoFactor()->x(), geoMat->albedoFactor()->y(),
                      geoMat->albedoFactor()->z(), geoMat->albedoFactor()->w()};
	
	// Albedo texture
  TextureType albedoTextureType = TextureType::kAlbedo;
  Handle albedoHandle =
      ::texturePool->Load(geoMat->albedoTexture()->c_str(), &albedoTextureType);
  if (::texturePool->IsValidHandle(albedoHandle))
  {
    mat.albedoTexture = albedoHandle;
	}

  mat.metallicFactor = geoMat->metallicFactor();
  mat.roughnessFactor = geoMat->roughnessFactor();

	// Metallic roughness texture
  TextureType metallicRoughnessTextureType = TextureType::kMetalicRoughness;
	Handle metallicRoughnessHandle =
      ::texturePool->Load(geoMat->metallicRoughnessTexture()->c_str(),
                          &metallicRoughnessTextureType);
  if (::texturePool->IsValidHandle(metallicRoughnessHandle))
  {
    mat.metallicRoughnessTexture = metallicRoughnessHandle;
  }

	// Normal texture
  TextureType normalTextureType = TextureType::kNormal;
  Handle normalHandle =
      ::texturePool->Load(geoMat->normalTexture()->c_str(), &normalTextureType);
  if (::texturePool->IsValidHandle(normalHandle))
  {
    mat.normalTexture = normalHandle;
  }

	// Ambient occlusion texture
  TextureType occlusionTextureType = TextureType::kAmbientOcclusion;
  Handle occlusionHandle = ::texturePool->Load(
      geoMat->occlusionTexture()->c_str(), &occlusionTextureType);
  if (::texturePool->IsValidHandle(occlusionHandle))
  {
    mat.occlusionTexture = occlusionHandle;
  }

  mat.emissiveFactor = geoMat->emissiveFactor();
	// Emissive texture
  TextureType emissiveTextureType = TextureType::kEmissive;
  Handle emissiveHandle = ::texturePool->Load(
      geoMat->emissiveTexture()->c_str(), &emissiveTextureType);
  if (::texturePool->IsValidHandle(emissiveHandle))
  {
    mat.emissiveTexture = emissiveHandle;
  }

  mat.alphaMode = static_cast<AlphaMode>(geoMat->alphaMode());
  mat.alphaCutoff = geoMat->alphaCutoff();
  mat.doubleSided = geoMat->doubleSided();
}

template<>
Handle ResourcePool<MaterialData>::LoadImpl(xUUID uuid, void* pUser)
{
  ::pools = (Pools*)pUser;
  ::texturePool = pools->texturePool;

  fs::path path = GetResourcePath(uuid);

  std::ifstream ifs(path, std::ios::binary);
  std::vector<char> buffer((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());

  const GameResource::Material* geoMat =
      flatbuffers::GetRoot<GameResource::Material>(buffer.data());

  MaterialData mat;
  ProcessMaterial(mat, geoMat);

	Handle handle = _handleTable.ClaimHandle(std::move(mat),
                                           (uint16_t)ResourceType::kMaterial);
  if (!_handleTable.IsValidHandle(handle))
  {
    std::cout << uuid.ToString() << "; material loading failed." << std::endl;
    terminate();
  }
  else
  {
    std::cout << uuid.ToString()
              << "; material loading succeeded: " << handle.index << ", "
              << handle.version << "." << std::endl;
  }
  _uuidMap[uuid] = handle.index;
  _handleUUIDMap[handle] = uuid;
  return handle;
}

template <>
void ResourcePool<MaterialData>::UnloadImpl(Handle& handle, void* pReserved)
{
  ::pools = (Pools*)pReserved;
  ::texturePool = pools->texturePool;

	std::cout << "material unloading succeeded : " << handle.index << ", "
            << handle.version << "." << std::endl;

  MaterialData& data = _handleTable[handle].value();

  if (::texturePool->IsValidHandle(data.albedoTexture))
  {
    ::texturePool->Unload(data.albedoTexture, ::pools);
  }

  if (::texturePool->IsValidHandle(data.metallicRoughnessTexture))
  {
    ::texturePool->Unload(data.metallicRoughnessTexture, ::pools);
  }

  if (::texturePool->IsValidHandle(data.normalTexture))
  {
    ::texturePool->Unload(data.normalTexture, ::pools);
  }

  if (::texturePool->IsValidHandle(data.occlusionTexture))
  {
    ::texturePool->Unload(data.occlusionTexture, ::pools);
  }

  if (::texturePool->IsValidHandle(data.emissiveTexture))
  {
    ::texturePool->Unload(data.emissiveTexture, ::pools);
  }
}


template <>
Handle ResourcePool<MaterialData>::DuplicateHandleImpl(const Handle& handle,
                                                    void* pReserved)
{
  ::pools = (Pools*)pReserved;
  ::texturePool = pools->texturePool;

  std::cout << "material unloading succeeded : " << handle.index << ", "
            << handle.version << "." << std::endl;

  MaterialData& data = _handleTable[handle].value();

  if (::texturePool->IsValidHandle(data.albedoTexture))
  {
    ::texturePool->DuplicateHandle(data.albedoTexture, ::pools);
  }

  if (::texturePool->IsValidHandle(data.metallicRoughnessTexture))
  {
    ::texturePool->DuplicateHandle(data.metallicRoughnessTexture, ::pools);
  }

  if (::texturePool->IsValidHandle(data.normalTexture))
  {
    ::texturePool->DuplicateHandle(data.normalTexture, ::pools);
  }

  if (::texturePool->IsValidHandle(data.occlusionTexture))
  {
    ::texturePool->DuplicateHandle(data.occlusionTexture, ::pools);
  }

  if (::texturePool->IsValidHandle(data.emissiveTexture))
  {
    ::texturePool->DuplicateHandle(data.emissiveTexture, ::pools);
  }

	return _handleTable.DuplicateHandle(handle);
}
