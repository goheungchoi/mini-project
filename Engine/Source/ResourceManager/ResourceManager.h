#pragma once

#include "Common.h"

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

  ResourceType (*GetResourceType)(const Handle& handle);
  bool (*IsValidHandle)(const Handle& handle);

  void (*UnloadAll)();
};

const ResourceManager* GetResourceManager();


inline ShaderHandle LoadShader(const std::string& path, ShaderType type)
{
  GetResourceManager()->LoadShader(path, type);
}

inline const ShaderData& AccessShaderData(ShaderHandle handle)
{
  return GetResourceManager()->AccessShaderData(handle);
}

inline void UnloadShader(ShaderHandle handle)
{
  GetResourceManager()->UnloadShader(handle);
}


inline TextureHandle LoadTexture(const std::string& path, TextureType type)
{
  GetResourceManager()->LoadTexture(path, type);
}

inline const TextureData& AccessTextureData(TextureHandle handle)
{
  return GetResourceManager()->AccessTextureData(handle);
}

inline void UnloadTexture(TextureHandle handle)
{
  GetResourceManager()->UnloadTexture(handle);
}


inline MaterialHandle LoadMaterial(const std::string& path)
{
  GetResourceManager()->LoadMaterial(path);
}

inline const MaterialData& AccessMaterialData(MaterialHandle handle)
{
  return GetResourceManager()->AccessMaterialData(handle);
}

inline void UnloadMaterial(MaterialHandle handle)
{
  GetResourceManager()->UnloadMaterial(handle);
}


inline MeshHandle LoadMesh(const std::string& path)
{
  GetResourceManager()->LoadMesh(path);
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
  GetResourceManager()->LoadModel(path);
}

inline const ModelData& AccessModelData(ModelHandle handle)
{
  return GetResourceManager()->AccessModelData(handle);
}

inline void UnloadModel(ModelHandle handle)
{
  GetResourceManager()->UnloadModel(handle);
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
