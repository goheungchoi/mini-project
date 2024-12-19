#pragma once

#include "Common.h"

struct ResourceManager {
  TextureHandle (*LoadTexture)(const std::string& path, TextureType type);
  const TextureData& (*AccessTextureData)(TextureHandle handle);
  void (*UnloadTexture)(TextureHandle& handle);

  ShaderHandle (*LoadShader)(const std::string& path, ShaderType type);
  const ShaderData& (*AccessShaderData)(ShaderHandle handle);
  void (*UnloadShader)(ShaderHandle handle);

	MeshHandle (*LoadMesh)(const std::string& path);
  const MeshData& (*AccessMeshData)(MeshHandle handle);
  void (*UnloadMesh)(MeshHandle handle);

	MaterialHandle (*LoadMaterial)(const std::string& path);
  const MaterialData& (*AccessMaterialData)(MaterialHandle handle);
  void (*UnloadMaterial)(MaterialHandle handle);

  ResourceType (*GetResourceType)(const Handle& handle);
  bool (*IsValidHandle)(const Handle& handle);

  void (*UnloadAll)();
};

const ResourceManager* GetResourceManager();

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

inline ResourceType GetResourceType(const Handle& handle) {
  GetResourceManager()->GetResourceType(handle);
}

inline bool IsValidHandle(const Handle& handle) {
  return GetResourceManager()->IsValidHandle(handle);
}

inline void UnloadAll() {
  GetResourceManager()->UnloadAll();
}
