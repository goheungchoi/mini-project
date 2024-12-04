#pragma once

#include "Common.h"

struct ResourceManager {
  Handle (*LoadTexture)(const std::string& path, TextureType type);
  const TextureData& (*AccessTextureData)(Handle handle);
  void (*UnloadTexture)(Handle& handle);

  Handle (*LoadShader)(const std::string& path, ShaderType type);
  const ShaderData& (*AccessShaderData)(Handle handle);
  void (*UnloadShader)(Handle handle);

	Handle (*LoadMesh)(const std::string& path);
  const MeshData& (*AccessMeshData)(Handle handle);
  void (*UnloadMesh)(Handle handle);

	Handle (*LoadMaterial)(const std::string& path);
  const MaterialData& (*AccessMaterialData)(Handle handle);
  void (*UnloadMaterial)(Handle handle);

  ResourceType (*GetResourceType)(const Handle& handle);
  bool (*IsValidHandle)(const Handle& handle);

  void (*UnloadAll)();
};

const ResourceManager* GetResourceManager();

inline Handle LoadTexture(const std::string& path, TextureType type) {
  GetResourceManager()->LoadTexture(path, type);
}

inline const TextureData& AccessTextureData(Handle handle) {
  return GetResourceManager()->AccessTextureData(handle);
}

inline void UnloadTexture(Handle handle) {
  GetResourceManager()->UnloadTexture(handle);
}

inline Handle LoadShader(const std::string& path, ShaderType type) {
  GetResourceManager()->LoadShader(path, type);
}

inline const ShaderData& AccessShaderData(Handle handle) {
  return GetResourceManager()->AccessShaderData(handle);
}

inline void UnloadShader(Handle handle) {
  GetResourceManager()->UnloadShader(handle);
}

inline Handle LoadMesh(const std::string& path) {
  GetResourceManager()->LoadMesh(path);
}

inline const MeshData& AccessMeshData(Handle handle) {
  return GetResourceManager()->AccessMeshData(handle);
}

inline void UnloadMesh(Handle handle) {
  GetResourceManager()->UnloadMesh(handle);
}

inline Handle LoadMaterial(const std::string& path) {
  GetResourceManager()->LoadMaterial(path);
}

inline const MaterialData& AccessMaterialData(Handle handle) {
  return GetResourceManager()->AccessMaterialData(handle);
}

inline void UnloadMaterial(Handle handle) {
  GetResourceManager()->UnloadMaterial(handle);
}

inline ResourceType GetResourceType(const Handle& handle) {
  GetResourceManager()->GetResourceType(handle);
}

inline bool IsValidHandle(const Handle& handle) {
  return GetResourceManager()->IsValidHandle(handle);
}

inline Handle UnloadAll(const std::string& path, ShaderType type) {
  GetResourceManager()->UnloadAll();
}
