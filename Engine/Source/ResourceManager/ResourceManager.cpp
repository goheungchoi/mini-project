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
  ResourcePool<SkeletonData> skeletonPool;
  ResourcePool<AnimationData> animationPool;
};

struct Pools
{
  ResourcePool<TextureData>* texturePool;
  ResourcePool<MaterialData>* materialPool;
  ResourcePool<MeshData>* meshPool;
  ResourcePool<SkeletonData>* skeletonPool;
  ResourcePool<AnimationData>* animationPool;
};
}


static ::__ResourceManagerPrivate__& _m() {
  static ::__ResourceManagerPrivate__ _resourceManagerPrivate;
  return _resourceManagerPrivate;
}

static ::Pools& _pools() {
  static ::Pools pools{&_m().texturePool, &_m().materialPool, &_m().meshPool,
                       &_m().skeletonPool, &_m().animationPool};
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

// Skeleton
static SkeletonHandle __LoadSkeleton__(const std::string& path)
{
  Handle skeletonHandle = _m().skeletonPool.Load(path.c_str(), &_pools());
  if (_m().skeletonPool.IsValidHandle(skeletonHandle))
  {
    return skeletonHandle;
  }
  return SkeletonHandle();
}
static const SkeletonData& __AccessSkeletonData__(SkeletonHandle handle)
{
  return _m().skeletonPool.AccessResourceData(handle);
}
static void __UnloadSkeleton__(SkeletonHandle handle) {}

// Animation
static AnimationHandle __LoadAnimation__(const std::string& path)
{
  Handle animationHandle = _m().animationPool.Load(path.c_str(), &_pools());
  if (_m().animationPool.IsValidHandle(animationHandle))
  {
    return animationHandle;
  }
  return AnimationHandle();
}
static const AnimationData& __AccessAnimationData__(AnimationHandle handle)
{
  return _m().animationPool.AccessResourceData(handle);
}
static void __UnloadAnimation__(AnimationHandle handle) {}

static ResourceType __GetResourceType__(const Handle& handle) {
  // TODO:
	return ResourceType::kUnknown;
}
static bool __IsValidHandle__(const Handle& handle) {
	// TODO:
  return false;
}

static void __UnloadAll__() {
  // TODO:
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

			.LoadSkeleton = __LoadSkeleton__,
			.AccessSkeletonData = __AccessSkeletonData__,
      .UnloadSkeleton = __UnloadSkeleton__,

      .LoadAnimation = __LoadAnimation__,
      .AccessAnimationData = __AccessAnimationData__,
      .UnloadAnimation = __UnloadAnimation__,

      .GetResourceType = __GetResourceType__,
      .IsValidHandle = __IsValidHandle__,

      .UnloadAll = __UnloadAll__};
	
	return &_resourceManager;
}


