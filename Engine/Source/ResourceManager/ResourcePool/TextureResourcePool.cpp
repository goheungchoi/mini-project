#include "ResourcePool.h"

#include "nvpro/texture_formats.h"
#define NV_DDS_UTILITY_VALUES
#include "nvpro/nv_dds.h"

#include "Core/Utils/IOUtils.h"

#include "Utils.h"

template <>
Handle ResourcePool<TextureData>::LoadImpl(xUUID uuid, void* pUser)
{
  TextureData data;

  fs::path path = GetResourcePath(uuid);
  std::vector<char> rawFile = ReadFile(path);

  nv_dds::Image image;
  nv_dds::ErrorWithText maybeError =
      image.readHeaderFromMemory(rawFile.data(), rawFile.size(), {});
  if (maybeError.has_value())
  {
    // TODO: Return a default texture!
    // throw std::exception(("Loading texture failed: " + maybeError.value()).c_str());
    return Handle::kInvalidHandle;
  }
  else
  {
    // Access subresources using image.subresource(...), and upload them
    // to the GPU using your graphics API of choice.

		data.type = *reinterpret_cast<TextureType*>(pUser);

    data.isSRGB = texture_formats::isDXGIFormatSRGB(image.dxgiFormat);
    data.isNormal = image.isNormal;
    data.isCubeMap =
        image.cubemapFaceFlags & nv_dds::DDSCAPS2_CUBEMAP_ALL_FACES;

		data.format = (DXGI_FORMAT) image.dxgiFormat;

		data.width = image.mip0Width;
    data.height = image.mip0Height;

		data.mipLevels = image.getNumMips();
    data.arrayLayers = image.getNumFaces();

		data.ddsData = reinterpret_cast<std::vector<uint8_t>&&>(std::move(rawFile));
  }

	// Claim the handle and map the UUID.
	Handle handle = _handleTable.ClaimHandle(
      std::move(data), (uint16_t)ResourceType::kTexture);
  if (!_handleTable.IsValidHandle(handle))
  {
    std::cout << uuid.ToString() << "; texture loading failed." << std::endl;
    terminate();
  }
  else
  {
    std::cout << uuid.ToString()
              << "; texture loading succeeded: " << handle.index << ", "
              << handle.version << "." << std::endl;
  }

  _uuidMap[uuid] = handle.index;
  _handleUUIDMap[handle] = uuid;
  return handle;
}

template<>
void ResourcePool<TextureData>::UnloadImpl(Handle& handle, void* pReserved)
{
  std::cout << "texture unloading succeeded : " << handle.index << ", "
            << handle.version << "." << std::endl;
}

template <>
Handle ResourcePool<TextureData>::DuplicateHandleImpl(const Handle& handle,
                                                       void* pReserved)
{
  return _handleTable.DuplicateHandle(handle);
}
