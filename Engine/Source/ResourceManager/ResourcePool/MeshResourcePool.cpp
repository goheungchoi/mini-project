#include "ResourcePool.h"

#include "Core/Utils/IOUtils.h"

#include "Utils.h"

template <>
Handle ResourcePool<MeshData>::LoadImpl(xUUID uuid, void* pUser)
{
	return Handle::kInvalidHandle;
}

