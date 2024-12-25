#include "ResourcePool.h"

#include "nvpro/nv_dds.h"

#include "Utils.h"

template <>
Handle ResourcePool<TextureData>::LoadImpl(xUUID uuid, void* pUser)
{
  TextureData data;

	// TODO:

  return Handle{};
}

template <>
bool ResourcePool<TextureData>::UnloadImpl(Handle& handle)
{
  return false;
}
