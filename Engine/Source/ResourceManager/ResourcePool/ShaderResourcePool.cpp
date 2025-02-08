#include "ResourcePool.h"

#include "Core/Types/ShaderData.h"

#include "Core/Utils/IOUtils.h"

#include "Utils.h"

#include <d3dcompiler.h>

template <>
Handle ResourcePool<ShaderData>::LoadImpl(xUUID uuid, void* pUser)
{
  ShaderData data;

	// Shader type
  ShaderType type = *reinterpret_cast<ShaderType*>(pUser);
  data.type = type;

	// Read the raw file
  fs::path path = GetResourcePath(uuid);
  std::vector<char> rawFile = ReadFile(path);

	// Check the profile
  LPCSTR profile;
  if (type == ShaderType::kVertex)
  {
    profile = "vs_5_0";
  }
  else if (type == ShaderType::kPixel)
  {
    profile = "ps_5_0";
  }
  else if (type == ShaderType::kCompute)
  {
    profile = "cs_5_0";
	}
  else
  {
    return Handle::kInvalidHandle;
  }

	// Compile the shader
  UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
  flags |= D3DCOMPILE_DEBUG;
  flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

  ID3DBlob* shaderBlob = nullptr;
  ID3DBlob* errorBlob = nullptr;
  HRESULT hr = D3DCompile(rawFile.data(), rawFile.size(), NULL, NULL,
                          D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", profile,
                          flags, 0, &shaderBlob, &errorBlob);
	// Error handling
  if (FAILED(hr))
  {
    if (errorBlob)
    {
      OutputDebugStringA((char*)errorBlob->GetBufferPointer());
      errorBlob->Release();
    }

    if (shaderBlob)
      shaderBlob->Release();

    return Handle::kInvalidHandle;
  }

	// Copy the compiled data
  data.data.resize(shaderBlob->GetBufferSize());
  memcpy(data.data.data(), shaderBlob->GetBufferPointer(),
         shaderBlob->GetBufferSize());

	// Release the shader blob
	shaderBlob->Release();

	// Claim the handle and map the UUID.
  Handle handle = _handleTable.ClaimHandle(std::move(data));
  _uuidMap[uuid] = handle.index;
  _handleUUIDMap[handle] = uuid;
  return handle;
}

template <>
void ResourcePool<ShaderData>::UnloadImpl(Handle& handle, void* pReserved)
{
}
