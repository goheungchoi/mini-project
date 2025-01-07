#pragma once

#include "Core/Handle/ResourceHandle.h"
#include "Core/Types/TextureData.h"
#include "Core/Types/MeshData.h"
#include "Core/Types/MaterialData.h"
#include "Core/Types/ShaderData.h"
#include "Core/Types/ModelData.h"

#include "Utils.h"

#include "Shared/Config/Config.h"

template <typename T, size_t GROW_SIZE = 1024>
class ResourcePool
{
  std::unordered_map<xUUID, uint32_t> _uuidMap;

	HandleTable<T, GROW_SIZE> _handleTable;

public:
  Handle Load(const char* path, void* pReserved)
  {
    xUUID uuid = GenerateUUIDFromName(path);

		// Check if the asset is already loaded
    if (auto it = _uuidMap.find(uuid); it == _uuidMap.end())
    {
      // Need to load a new resource
      return LoadImpl(uuid, pReserved);
    }
    else
    {
      // Duplicate the handle
      uint32_t index = it->second;
      Handle handle = _handleTable[index];
      return _handleTable.DuplicateHandle(handle);
    }

    return Handle::kInvalidHandle;
  }

  void Unload(Handle& handle, void* pReserved)
  {
    if (_handleTable.GetReferenceCount(handle) == 1)
    {
      UnloadImpl(handle, pReserved);
		}

    // Release the handle
    _handleTable.ReleaseHandle(handle);
  }

  bool IsValidHandle(const Handle& handle) const
  {
    return _handleTable.IsValidHandle(handle);
  }

  const T& AccessResourceData(const Handle& handle) const
  {
    if (!IsValidHandle(handle))
      // TODO: Error message
      throw std::exception("Invalid handle!");

    return _handleTable[handle].value();
  }

private:
  Handle LoadImpl(xUUID uuid, void* pReserved)
  {
    return Handle::kInvalidHandle;
  }

	void UnloadImpl(Handle& handle, void* pReserved) { return; }
};

template <>
Handle ResourcePool<ShaderData>::LoadImpl(xUUID uuid, void* pUser);
template <>
Handle ResourcePool<TextureData>::LoadImpl(xUUID uuid, void* pUser);
template <>
Handle ResourcePool<MaterialData>::LoadImpl(xUUID uuid, void* pUser);
template <>
Handle ResourcePool<MeshData>::LoadImpl(xUUID uuid, void* pUser);
template <>
Handle ResourcePool<ModelData>::LoadImpl(xUUID uuid, void* pUser);


// TODO;
//template <>
//void ResourcePool<TextureData>::UnloadImpl(Handle& uuid, void* pUser);
//template <>
//void ResourcePool<ShaderData>::UnloadImpl(Handle& uuid, void* pUser);
//template <>
//void ResourcePool<MeshData>::UnloadImpl(Handle& uuid, void* pUser);
//template <>
//void ResourcePool<ModelData>::UnloadImpl(Handle& uuid, void* pUser);
