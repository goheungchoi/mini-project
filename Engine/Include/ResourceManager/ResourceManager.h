#pragma once

#include "Core/Handle/Handle.h"
#include "Core/Handle/ResourceHandle.h"

#include "Core/Types/ShaderData.h"
#include "Core/Types/MeshData.h"
#include "Core/Types/ModelData.h"
#include "Core/Types/MaterialData.h"
#include "Core/Types/TextureData.h"
#include "Core/Types/SkeletonData.h"
#include "Core/Types/AnimationData.h"

struct ResourceManager {
  
  ShaderHandle (*LoadShader)(const std::string& path, ShaderType type);
  const ShaderData& (*AccessShaderData)(ShaderHandle handle);
  void (*UnloadShader)(ShaderHandle handle);

	TextureHandle (*LoadTexture)(const std::string& path, TextureType type);
  const TextureData& (*AccessTextureData)(TextureHandle handle);
  void (*UnloadTexture)(TextureHandle& handle);

	MaterialHandle (*LoadMaterial)(const std::string& path);
  const MaterialData& (*AccessMaterialData)(MaterialHandle handle);
  void (*UnloadMaterial)(MaterialHandle handle);

	MeshHandle (*LoadMesh)(const std::string& path);
  const MeshData& (*AccessMeshData)(MeshHandle handle);
  void (*UnloadMesh)(MeshHandle handle);

	ModelHandle (*LoadModel)(const std::string& path);
  const ModelData& (*AccessModelData)(ModelHandle handle);
  void (*UnloadModel)(ModelHandle handle);

	SkeletonHandle (*LoadSkeleton)(const std::string& path);
  const SkeletonData& (*AccessSkeletonData)(SkeletonHandle handle);
  void (*UnloadSkeleton)(SkeletonHandle handle);

	AnimationHandle (*LoadAnimation)(const std::string& path);
  const AnimationData& (*AccessAnimationData)(AnimationHandle handle);
  void (*UnloadAnimation)(AnimationHandle handle);

  ResourceType (*GetResourceType)(const Handle& handle);
  bool (*IsValidHandle)(const Handle& handle);

  void (*UnloadAll)();
};

const ResourceManager* GetResourceManager();

// Shader
inline ShaderHandle LoadShader(const std::string& path, ShaderType type)
{
  return GetResourceManager()->LoadShader(path, type);
}

inline const ShaderData& AccessShaderData(ShaderHandle handle)
{
  return GetResourceManager()->AccessShaderData(handle);
}

inline void UnloadShader(ShaderHandle handle)
{
  GetResourceManager()->UnloadShader(handle);
}

// Texture
inline TextureHandle LoadTexture(const std::string& path, TextureType type)
{
  return GetResourceManager()->LoadTexture(path, type);
}

inline const TextureData& AccessTextureData(TextureHandle handle)
{
  return GetResourceManager()->AccessTextureData(handle);
}

inline void UnloadTexture(TextureHandle handle)
{
  GetResourceManager()->UnloadTexture(handle);
}

// Material
inline MaterialHandle LoadMaterial(const std::string& path)
{
  return GetResourceManager()->LoadMaterial(path);
}

inline const MaterialData& AccessMaterialData(MaterialHandle handle)
{
  return GetResourceManager()->AccessMaterialData(handle);
}

inline void UnloadMaterial(MaterialHandle handle)
{
  GetResourceManager()->UnloadMaterial(handle);
}

// Mesh
inline MeshHandle LoadMesh(const std::string& path)
{
  return GetResourceManager()->LoadMesh(path);
}

inline const MeshData& AccessMeshData(MeshHandle handle)
{
  return GetResourceManager()->AccessMeshData(handle);
}

inline void UnloadMesh(MeshHandle handle)
{
  GetResourceManager()->UnloadMesh(handle);
}

/**
 * @brief Load the model
 * @param path The relative path to the model data from the Asset directory.
 * @return Invalid handle if not available, otherwise, returns a valid handle.
 */
inline ModelHandle LoadModel(const std::string& path)
{
  return GetResourceManager()->LoadModel(path);
}

inline const ModelData& AccessModelData(ModelHandle handle)
{
  return GetResourceManager()->AccessModelData(handle);
}

inline void UnloadModel(ModelHandle handle)
{
  GetResourceManager()->UnloadModel(handle);
}


// Skeleton
inline SkeletonHandle LoadSkeleton(const std::string& path)
{
  return GetResourceManager()->LoadSkeleton(path);
}

inline const SkeletonData& AccessSkeletonData(SkeletonHandle handle)
{
  return GetResourceManager()->AccessSkeletonData(handle);
}

inline void UnloadSkeleton(SkeletonHandle handle)
{
  GetResourceManager()->UnloadSkeleton(handle);
}

// Animation
inline AnimationHandle LoadAnimation(const std::string& path)
{
  return GetResourceManager()->LoadAnimation(path);
}

inline const AnimationData& AccessAnimationData(AnimationHandle handle)
{
  return GetResourceManager()->AccessAnimationData(handle);
}

inline void UnloadAnimation(AnimationHandle handle)
{
  GetResourceManager()->UnloadAnimation(handle);
}


inline ResourceType GetResourceType(const Handle& handle) {
  GetResourceManager()->GetResourceType(handle);
}

inline bool IsValidHandle(const Handle& handle) {
  return GetResourceManager()->IsValidHandle(handle);
}

inline void UnloadAll() {
  GetResourceManager()->UnloadAll();
}
