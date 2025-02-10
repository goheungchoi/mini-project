#pragma once

#include "Core/Handle/ResourceHandle.h"
#include "Core/Types/TextureData.h"
#include "Core/Types/MeshData.h"
#include "Core/Types/MaterialData.h"
#include "Core/Types/ShaderData.h"
#include "Core/Types/ModelData.h"
#include "Core/Types/SkeletonData.h"
#include "Core/Types/AnimationData.h"

#include "Utils.h"

#include "Shared/Config/Config.h"

#include <mutex>

template <typename T, size_t GROW_SIZE = 1024>
class ResourcePool
{
  // mutex
  mutable std::mutex _resourcePoolMutex;

  std::unordered_map<xUUID, uint32_t> _uuidMap;
  std::unordered_map<Handle, xUUID> _handleUUIDMap;

	HandleTable<T, GROW_SIZE> _handleTable;

public:

  Handle Load(const char* path, void* pReserved)
  {
    xUUID uuid = GenerateUUIDFromName(path);

    if (uuid.IsNil())
    {
      std::cout << typeid(T).name() << " ResourcePool; path is null."
                << std::endl;
      return Handle::kInvalidHandle;
    }

		// Lock the thread for safe multithreading.
    std::lock_guard<std::mutex> lock(_resourcePoolMutex);

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

			std::cout << "Duplicate " << handle.index << ", " << handle.version
                << ", " << handle.desc << std::endl;

      return DuplicateHandleImpl(handle, pReserved);
    }

    return Handle::kInvalidHandle;
  }

	void DiscardDetailedData(Handle& handle, void* pReserved) {
    // Lock the thread for safe multithreading.
    std::lock_guard<std::mutex> lock(_resourcePoolMutex);

    DiscardDetailedDataImpl(handle, pReserved);
	}

	void RestoreDetailedData(Handle& handle, void* pReserved) {
    // Lock the thread for safe multithreading.
    std::lock_guard<std::mutex> lock(_resourcePoolMutex);

    RestoreDetailedDataImpl(handle, pReserved);
	}

  void Unload(Handle& handle, void* pReserved)
  {
    // Lock the thread for safe multithreading.
    std::lock_guard<std::mutex> lock(_resourcePoolMutex);

    if (!DeadlockFree_IsValidHandle(handle))
    {
      // TODO: Error message
      std::cout << typeid(T).name() << "; unloading failed: invalid handle."
                << std::endl;
			throw std::exception("Invalid handle!");
		}

    // Temp
    Handle tmp = handle;

    // Specialized unloading logic implementation.
    UnloadImpl(handle, pReserved);

    // Release the handle
    _handleTable.ReleaseHandle(handle);

    // Erase the map histories.
    if (_handleTable.GetReferenceCount(tmp) == 0)
    {
      xUUID uuid = _handleUUIDMap[tmp];
      auto it = _uuidMap.find(uuid);
      _uuidMap.erase(it);
      _handleUUIDMap.erase(tmp);
    }
  }

	Handle Clone(const Handle& handle) { 
		// Lock the thread for safe multithreading.
    std::lock_guard<std::mutex> lock(_resourcePoolMutex);

		if (!DeadlockFree_IsValidHandle(handle))
      // TODO: Error message
      throw std::exception("Invalid handle!");

    return _handleTable.Clone(handle);
	}

  bool IsValidHandle(const Handle& handle) const
  {
    // Lock the thread for safe multithreading.
    std::lock_guard<std::mutex> lock(_resourcePoolMutex);
    return _handleTable.IsValidHandle(handle);
  }

  T& AccessResourceData(const Handle& handle)
  {
    // Lock the thread for safe multithreading.
    std::lock_guard<std::mutex> lock(_resourcePoolMutex);

		if (!DeadlockFree_IsValidHandle(handle))
		{
      // TODO: Error message
      std::cout << typeid(T).name() << "; access resource data failed." << std::endl;
      throw std::exception("Invalid handle!");
		}

    return _handleTable[handle].value();
  }

	Handle DuplicateHandle(const Handle& handle, void* pReserved)
  {
    // Lock the thread for safe multithreading.
    std::lock_guard<std::mutex> lock(_resourcePoolMutex);

		if (!DeadlockFree_IsValidHandle(handle))
    {
      // TODO: Error message
      std::cout << typeid(T).name() << "; duplicate handle failed."
                << std::endl;
      throw std::exception("Invalid handle!");
    }

    return DuplicateHandleImpl(handle, pReserved);
	}

private:
  bool DeadlockFree_IsValidHandle(const Handle& handle) const {
    return _handleTable.IsValidHandle(handle);
  }

  Handle LoadImpl(xUUID uuid, void* pReserved)
  {
    return Handle::kInvalidHandle;
  }

	void DiscardDetailedDataImpl(Handle& handle, void* pReserved) {}

	void RestoreDetailedDataImpl(Handle& handle, void* pReserved) {}

	void UnloadImpl(Handle& handle, void* pReserved) { return; }

	Handle DuplicateHandleImpl(const Handle& handle, void* pReserved)
  {
    return Handle::kInvalidHandle;
  }
};

template <>
Handle ResourcePool<ShaderData>::LoadImpl(xUUID uuid, void* pReserved);
template <>
Handle ResourcePool<TextureData>::LoadImpl(xUUID uuid, void* pReserved);
template <>
Handle ResourcePool<MaterialData>::LoadImpl(xUUID uuid, void* pReserved);
template <>
Handle ResourcePool<MeshData>::LoadImpl(xUUID uuid, void* pReserved);
template <>
Handle ResourcePool<ModelData>::LoadImpl(xUUID uuid, void* pReserved);
template <>
Handle ResourcePool<SkeletonData>::LoadImpl(xUUID uuid, void* pReserved);
template <>
Handle ResourcePool<AnimationData>::LoadImpl(xUUID uuid, void* pReserved);

template <>
void ResourcePool<ShaderData>::UnloadImpl(Handle& handle, void* pReserved);
template <>
void ResourcePool<TextureData>::UnloadImpl(Handle& handle, void* pReserved);
template <>
void ResourcePool<MaterialData>::UnloadImpl(Handle& handle, void* pReserved);
template <>
void ResourcePool<MeshData>::UnloadImpl(Handle& handle, void* pReserved);
template <>
void ResourcePool<ModelData>::UnloadImpl(Handle& handle, void* pReserved);
template <>
void ResourcePool<SkeletonData>::UnloadImpl(Handle& handle, void* pReserved);
template <>
void ResourcePool<AnimationData>::UnloadImpl(Handle& handle, void* pReserved);

template <>
Handle ResourcePool<ShaderData>::DuplicateHandleImpl(const Handle& handle,
                                                     void* pReserved);
template <>
Handle ResourcePool<TextureData>::DuplicateHandleImpl(const Handle& handle,
                                                      void* pReserved);
template <>
Handle ResourcePool<MaterialData>::DuplicateHandleImpl(const Handle& handle,
                                                       void* pReserved);
template <>
Handle ResourcePool<MeshData>::DuplicateHandleImpl(const Handle& handle,
                                                   void* pReserved);
template <>
Handle ResourcePool<ModelData>::DuplicateHandleImpl(const Handle& handle,
                                                    void* pReserved);
template <>
Handle ResourcePool<SkeletonData>::DuplicateHandleImpl(const Handle& handle,
                                                       void* pReserved);
template <>
Handle ResourcePool<AnimationData>::DuplicateHandleImpl(const Handle& handle,
                                                        void* pReserved);

// TODO: Restore and discard detailed data.
