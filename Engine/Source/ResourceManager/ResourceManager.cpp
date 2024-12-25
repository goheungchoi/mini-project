#include "ResourceManager.h"

// TODO: Goheung Choi
#include "ResourcePool/ResourcePool.h"

struct __ResourceManagerPrivate__
{
  ResourcePool<TextureData> texturePool;
  ResourcePool<ShaderData> shaderPool;
  ResourcePool<MeshData> meshPool;
  ResourcePool<MaterialData> materialPool;
  ResourcePool<ModelData> modelPool;
};

const fs::path kResourceDir{GetResourceDirectory()};

static __ResourceManagerPrivate__& _m() {
  static __ResourceManagerPrivate__ _resourceManagerPrivate;
  return _resourceManagerPrivate;
}

static TextureHandle __LoadTexture__(const std::string& path, TextureType type) {
  return TextureHandle();
}
static const TextureData& __AccessTextureData__(TextureHandle handle) {
  return TextureData();
}

static void __UnloadTexture__(TextureHandle& handle) {
}

static ShaderHandle __LoadShader__(const std::string& path, ShaderType type) {
  return ShaderHandle();
}
static const ShaderData& __AccessShaderData__(ShaderHandle handle) {
  return ShaderData();
}
static void __UnloadShader__(ShaderHandle handle) {
}

static MeshHandle __LoadMesh__(const std::string& path) {
  return MeshHandle();
}
static const MeshData& __AccessMeshData__(MeshHandle handle) {
  return MeshData();
}
static void __UnloadMesh__(MeshHandle handle) {
}

static MaterialHandle __LoadMaterial__(const std::string& path) {
	// TODO:
	// Load the material data

	// Load textures used in the material

  return MaterialHandle();
}
static const MaterialData& __AccessMaterialData__(MaterialHandle handle) {
  return MaterialData();
}
static void __UnloadMaterial__(MaterialHandle handle) {
}
static ResourceType __GetResourceType__(const Handle& handle) {
	return ResourceType::kUnknown;
}
static bool __IsValidHandle__(const Handle& handle) {
  return false;
}

static void __UnloadAll__() {
}


const ResourceManager* GetResourceManager() { 
	static ResourceManager _resourceManager
  {
    .LoadTexture = __LoadTexture__, .AccessTextureData = __AccessTextureData__,
    .UnloadTexture = __UnloadTexture__,

    .LoadShader = __LoadShader__, .AccessShaderData = __AccessShaderData__,
    .UnloadShader = __UnloadShader__,

    .LoadMesh = __LoadMesh__, .AccessMeshData = __AccessMeshData__,
    .UnloadMesh = __UnloadMesh__,

    .LoadMaterial = __LoadMaterial__, .AccessMaterialData = __AccessMaterialData__,
    .UnloadMaterial = __UnloadMaterial__,

    .GetResourceType = __GetResourceType__, .IsValidHandle = __IsValidHandle__,

    .UnloadAll = __UnloadAll__
  };
	
	return &_resourceManager;
}


