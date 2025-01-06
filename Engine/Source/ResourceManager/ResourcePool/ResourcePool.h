#pragma once

#include "Core/Handle/ResourceHandle.h"
#include "Core/Types/TextureData.h"
#include "Core/Types/MeshData.h"
#include "Core/Types/MaterialData.h"
#include "Core/Types/ShaderData.h"

#include "Utils.h"

template <typename T, size_t GROW_SIZE = 1024>
class ResourcePool
{
  std::unordered_map<xUUID, uint32_t> _uuidMap;

	HandleTable<T, GROW_SIZE> _handleTable;

public:
  Handle Load(const char* path, void* pUser)
  {
    xUUID uuid = GenerateUUIDFromName(path);

		// Check if the asset is already loaded
    if (auto it = _uuidMap.find(uuid); it == _uuidMap.end())
    {
      // Need to load a new resource
      return LoadImpl(uuid, pUser);
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

  void Unload(Handle& handle)
  {
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


  Handle LoadImpl(xUUID uuid, void* pUser)
  {
    return Handle::kInvalidHandle;
  }

  bool UnloadImpl(Handle& handle)
  {
    return false;
  }
};

template <>
Handle ResourcePool<TextureData>::LoadImpl(xUUID uuid, void* pUser);
template <>
Handle ResourcePool<ShaderData>::LoadImpl(xUUID uuid, void* pUser);
template <>
Handle ResourcePool<MeshData>::LoadImpl(xUUID uuid, void* pUser);

template <>
bool ResourcePool<TextureData>::UnloadImpl(Handle& handle);
