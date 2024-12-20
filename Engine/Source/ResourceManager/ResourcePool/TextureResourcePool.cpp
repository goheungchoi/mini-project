#include "ResourcePool.h"

#include "nvpro/nv_dds.h"

template <>
bool ResourcePool<TextureData>::LoadImpl(Handle& handle, xUUID uuid,
                                         void* pUser)
{
  return false;
}

template <>
bool ResourcePool<TextureData>::UnloadImpl(Handle& handle)
{
  return false;
}
