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
  ShaderData& (*AccessShaderData)(ShaderHandle handle);
  void (*UnloadShader)(ShaderHandle handle);

	TextureHandle (*LoadTexture)(const std::string& path, TextureType type);
  TextureData& (*AccessTextureData)(TextureHandle handle);
  void (*UnloadTexture)(TextureHandle& handle);

	MaterialHandle (*LoadMaterial)(const std::string& path);
  MaterialData& (*AccessMaterialData)(MaterialHandle handle);
  void (*UnloadMaterial)(MaterialHandle handle);

	MeshHandle (*LoadMesh)(const std::string& path);
  MeshData& (*AccessMeshData)(MeshHandle handle);
  MeshHandle (*CloneMesh)(MeshHandle handle);
  void (*UnloadMesh)(MeshHandle handle);

	ModelHandle (*LoadModel)(const std::string& path);
  ModelData& (*AccessModelData)(ModelHandle handle);
  ModelHandle (*CloneModel)(ModelHandle handle);
  void (*UnloadModel)(ModelHandle handle);

	SkeletonHandle (*LoadSkeleton)(const std::string& path);
  SkeletonData& (*AccessSkeletonData)(SkeletonHandle handle);
  void (*UnloadSkeleton)(SkeletonHandle handle);

	AnimationHandle (*LoadAnimation)(const std::string& path);
  AnimationData& (*AccessAnimationData)(AnimationHandle handle);
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

inline ShaderData& AccessShaderData(ShaderHandle handle)
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

inline TextureData& AccessTextureData(TextureHandle handle)
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

inline MaterialData& AccessMaterialData(MaterialHandle handle)
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

inline MeshData& AccessMeshData(MeshHandle handle)
{
  return GetResourceManager()->AccessMeshData(handle);
}

inline MeshHandle CloneMesh(MeshHandle handle) {
  return GetResourceManager()->CloneMesh(handle);
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

inline ModelData& AccessModelData(ModelHandle handle)
{
  return GetResourceManager()->AccessModelData(handle);
}

inline ModelHandle CloneModel(ModelHandle handle) {
  return GetResourceManager()->CloneModel(handle);
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

inline SkeletonData& AccessSkeletonData(SkeletonHandle handle)
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

inline AnimationData& AccessAnimationData(AnimationHandle handle)
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
