#include "ResourceManager/ResourceManager.h"

// TODO: Goheung Choi
#include "ResourcePool/ResourcePool.h"

namespace
{
struct __ResourceManagerPrivate__
{
  ResourcePool<ShaderData> shaderPool;
  ResourcePool<TextureData> texturePool;
  ResourcePool<MaterialData> materialPool;
  ResourcePool<MeshData> meshPool;
  ResourcePool<ModelData> modelPool;
};

struct Pools
{
  ResourcePool<TextureData>* texturePool;
  ResourcePool<MaterialData>* materialPool;
  ResourcePool<MeshData>* meshPool;
};
}


static ::__ResourceManagerPrivate__& _m() {
  static ::__ResourceManagerPrivate__ _resourceManagerPrivate;
  return _resourceManagerPrivate;
}

static ::Pools& _pools() {
  static ::Pools pools{&_m().texturePool, &_m().materialPool, &_m().meshPool};
  return pools;
}

static ShaderHandle __LoadShader__(const std::string& path, ShaderType type) {
  return ShaderHandle();
}
static const ShaderData& __AccessShaderData__(ShaderHandle handle) {
  return ShaderData();
}
static void __UnloadShader__(ShaderHandle handle) {
}

static TextureHandle __LoadTexture__(const std::string& path, TextureType type)
{
  Handle textureHandle = _m().texturePool.Load(path.c_str(), &type);
  if (_m().texturePool.IsValidHandle(textureHandle))
  {
    return textureHandle;
  }
  return TextureHandle();
}
static const TextureData& __AccessTextureData__(TextureHandle handle)
{
  return _m().texturePool.AccessResourceData(handle);
}
static void __UnloadTexture__(TextureHandle& handle) {
  _m().texturePool.Unload(handle, nullptr);
}

static MaterialHandle __LoadMaterial__(const std::string& path) {
  Handle matHandle = _m().materialPool.Load(path.c_str(), &_pools());
  if (_m().materialPool.IsValidHandle(matHandle))
  {
    return matHandle;
  }
  return MaterialHandle();
}
static const MaterialData& __AccessMaterialData__(MaterialHandle handle) {
  return _m().materialPool.AccessResourceData(handle);
}
static void __UnloadMaterial__(MaterialHandle handle) {
	// TODO;
}

static MeshHandle __LoadMesh__(const std::string& path)
{
  Handle meshHandle = _m().meshPool.Load(path.c_str(), &_pools());
  if (_m().meshPool.IsValidHandle(meshHandle))
  {
    return meshHandle;
	}
  return MeshHandle();
}
static const MeshData& __AccessMeshData__(MeshHandle handle)
{
  return _m().meshPool.AccessResourceData(handle);
}
static void __UnloadMesh__(MeshHandle handle) {
	// TODO:
}

static ModelHandle __LoadModel__(const std::string& path)
{
  Handle modelHandle = _m().modelPool.Load(path.c_str(), &_pools());
  if (_m().modelPool.IsValidHandle(modelHandle))
  {
    return modelHandle;
	}
  return ModelHandle();
}
static const ModelData& __AccessModelData__(ModelHandle handle)
{
  return _m().modelPool.AccessResourceData(handle);
}
static void __UnloadModel__(ModelHandle handle) {}

static ResourceType __GetResourceType__(const Handle& handle) {
	return ResourceType::kUnknown;
}
static bool __IsValidHandle__(const Handle& handle) {
  return false;
}

static void __UnloadAll__() {
}


const ResourceManager* GetResourceManager() { 
	static ResourceManager _resourceManager{

      .LoadShader = __LoadShader__,
      .AccessShaderData = __AccessShaderData__,
      .UnloadShader = __UnloadShader__,

      .LoadTexture = __LoadTexture__,
      .AccessTextureData = __AccessTextureData__,
      .UnloadTexture = __UnloadTexture__,

      .LoadMaterial = __LoadMaterial__,
      .AccessMaterialData = __AccessMaterialData__,
      .UnloadMaterial = __UnloadMaterial__,

      .LoadMesh = __LoadMesh__,
      .AccessMeshData = __AccessMeshData__,
      .UnloadMesh = __UnloadMesh__,

			.LoadModel = __LoadModel__,
			.AccessModelData = __AccessModelData__,
			.UnloadModel = __UnloadModel__,

      .GetResourceType = __GetResourceType__,
      .IsValidHandle = __IsValidHandle__,

      .UnloadAll = __UnloadAll__};
	
	return &_resourceManager;
}


