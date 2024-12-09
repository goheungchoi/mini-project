#pragma once

#include "Core/Handle/ResourceHandle.h"
#include "Core/Types/TextureData.h"
#include "Core/Types/MeshData.h"
#include "Core/Types/MaterialData.h"
#include "Core/Types/ShaderData.h"

template <typename T, std::size_t GROW_SIZE = 1024>
class ResourcePool
{
  using TableEntry = std::pair<Handle, std::optional<T>>;

  std::queue<std::size_t> _emptySlotQueue;

  std::unordered_map<UUID, u32> _uuidMap;

private:
  std::vector<u32> _refCounts;
  std::vector<TableEntry> _table;

public:
  Handle Load(const char* path, void* pUser)
  {
    UUID uuid = GenerateUUID_v5(path);

    if (auto it = _uuidMap.find(uuid); it == _uuidMap.end())
    {
      // New resource

      // Claim a handle
      Handle& handle{ClaimHandle()};

      if (!LoadImpl(handle, uuid, pUser))
      {
        return Handle::INVALID_HANDLE;
      }

      return DuplicateHandle(handle);
    }
    else
    {
      // Duplicate the handle
      u32 index = it->second;
      return DuplicateHandle(_table[index].first);
    }

    return false;
  }

  void Unload(Handle& handle)
  {

    // If this is the last resource to be unloaded
    if (_refCounts[handle.index] == 1)
    {
      // Unload the resource
      if (!UnloadImpl(handle))
        throw std::exception("unloading resource failed!");
    }

    // Release the handle
    ReleaseHandle(handle);
  }

  bool IsValidHandle(const Handle& handle) const
  {
    if (handle == Handle::INVALID_HANDLE || handle.index < _table.size())
      return false;
    // To be valid, the version and description has to be the same
    const Handle& valid = _table[handle.index].first;
    return _table[handle.index].second.has_value() && // Value existance check
           handle.version == valid.version &&         // Handle out-dated check
           handle.desc == valid.desc;                 // Handle type correction check
  }

  const T& AccessResourceData(const Handle& handle) const
  {
    if (!IsValidHandle(handle))
      // TODO: Error message
      throw std::exception("Invalid handle!");

    return _table[handle.index].second.value();
  }

private:
  void IncreaseCapacity()
  {
    std::size_t oldsize = _table.size();
    if (oldsize >= Handle::MAX_INDICES)
    {
      throw std::exception("max. resource capacity reached!");
    }

    std::size_t newsize = oldsize + GROW_SIZE;
    newsize = (newsize < Handle::MAX_INDICES) ? newsize : Handle::MAX_INDICES;

    _table.resize(newsize);
    _refCounts.resize(newsize);
  }

  Handle& ClaimHandle()
  {
    // TODO: Can be optimized with a mem queue.
    u64 i = 0;
    for (; i < _table.size(); ++i)
    {
      // Find the next empty spot
      if (_table[i].second == std::nullopt)
      {
        Handle& curr = _table[i].first;
        if (curr == Handle::INVALID_HANDLE)
        {
          curr.index = i;
          curr.version = Handle::MIN_VERSION;
        }
        else
        {
          u64 version = curr.version + 1;
          if (version > Handle::MAX_VERSION)
          {
            version = Handle::MIN_VERSION;
          }
          curr.version = version;
        }

        _refCounts[i] = 0;
        return curr;
      }
    }

    IncreaseCapacity();

    Handle& curr = _table[i].first;
    curr.index = i;
    curr.version = Handle::MIN_VERSION;

    _refCounts[i] = 0;
    return curr;
  }

  void ReleaseHandle(Handle& handle)
  {
    if (!IsValidHandle(handle))
      throw std::exception("invalid handle!");

    handle = Handle::INVALID_HANDLE;
    _refCounts[handle.index]--;
  }

  bool LoadImpl(Handle& handle, UUID uuid, void* pUser)
  {
    return false;
  }

  bool UnloadImpl(Handle& handle)
  {
    return false;
  }

  bool SetTableEntry(Handle& handle, T&& t)
  {
    if (_table[handle.index].second.has_value())
      return false;

    _table[handle.index].second = std::move(t);
    return true;
  }
};

template <>
bool ResourcePool<TextureData>::LoadImpl(Handle& handle, UUID uuid, void* pUser);

template <>
bool ResourcePool<TextureData>::UnloadImpl(Handle& handle);
