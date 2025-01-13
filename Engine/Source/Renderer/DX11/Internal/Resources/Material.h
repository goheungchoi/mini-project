#pragma once
#include "../Common.h"
#include "../Device.h"
#include "Core/Common.h"
#include "ResourceManager/ResourceManager.h"
using namespace Microsoft::WRL;

namespace Renderer
{
enum TextureType
{
  albedo = 0,
  normal,
  metallicRoughness,
  emissive,
  End
};
} // namespace Renderer

class Texture
{
private:
  ComPtr<ID3D11ShaderResourceView> _resource;

public:
  Texture() {}
  ~Texture() {}

public:
  static Texture* CreateSRV(Device* device, TextureData data)
  {
    Texture* pInstance = new Texture;

    // DirectXTex 라이브러리로 DDS 텍스처 생성
    ComPtr<ID3D11Resource> textureResource;
    HR_T(DirectX::CreateDDSTextureFromMemory(
        device->GetDevice(),
        data.ddsData.data(),                   // DDS 데이터 포인터
        data.ddsData.size(),                   // DDS 데이터 크기
        textureResource.GetAddressOf(),        // 생성된 리소스 반환
        pInstance->_resource.GetAddressOf())); // 생성된 SRV 반환

    return pInstance;
  }

  ComPtr<ID3D11ShaderResourceView> GetResource() { return _resource; }

private:
  uint32_t CalculateSysMemPitch(uint32_t width, DXGI_FORMAT format)
  {
    switch (format)
    {
    // Uncompressed formats
    case DXGI_FORMAT_R8_UNORM:
    case DXGI_FORMAT_R8_UINT:
      return width * 1; // 1 byte per pixel

    case DXGI_FORMAT_R16_UNORM:
    case DXGI_FORMAT_R16_UINT:
    case DXGI_FORMAT_R16_FLOAT:
      return width * 2; // 2 bytes per pixel

    case DXGI_FORMAT_R8G8_UNORM:
    case DXGI_FORMAT_R8G8_UINT:
      return width * 2; // 2 bytes per pixel

    case DXGI_FORMAT_R32_FLOAT:
    case DXGI_FORMAT_R32_UINT:
    case DXGI_FORMAT_R32_SINT:
      return width * 4; // 4 bytes per pixel

    case DXGI_FORMAT_R16G16_FLOAT:
    case DXGI_FORMAT_R16G16_UNORM:
    case DXGI_FORMAT_R16G16_UINT:
      return width * 4; // 4 bytes per pixel

    case DXGI_FORMAT_R8G8B8A8_UNORM:
    case DXGI_FORMAT_R8G8B8A8_UINT:
    case DXGI_FORMAT_B8G8R8A8_UNORM:
      return width * 4; // 4 bytes per pixel

    case DXGI_FORMAT_R32G32_FLOAT:
    case DXGI_FORMAT_R32G32_UINT:
      return width * 8; // 8 bytes per pixel

    case DXGI_FORMAT_R32G32B32_FLOAT:
      return width * 12; // 12 bytes per pixel

    case DXGI_FORMAT_R32G32B32A32_FLOAT:
    case DXGI_FORMAT_R32G32B32A32_UINT:
      return width * 16; // 16 bytes per pixel

    // Block-compressed formats
    case DXGI_FORMAT_BC1_UNORM:
    case DXGI_FORMAT_BC1_UNORM_SRGB:
    case DXGI_FORMAT_BC4_UNORM:
    case DXGI_FORMAT_BC4_SNORM:
      return ((width + 3) / 4) * 8; // 8 bytes per 4x4 block

    case DXGI_FORMAT_BC2_UNORM:
    case DXGI_FORMAT_BC2_UNORM_SRGB:
    case DXGI_FORMAT_BC3_UNORM:
    case DXGI_FORMAT_BC3_UNORM_SRGB:
    case DXGI_FORMAT_BC5_UNORM:
    case DXGI_FORMAT_BC5_SNORM:
    case DXGI_FORMAT_BC6H_UF16:
    case DXGI_FORMAT_BC6H_SF16:
    case DXGI_FORMAT_BC7_UNORM:
    case DXGI_FORMAT_BC7_UNORM_SRGB:
      return ((width + 3) / 4) * 16; // 16 bytes per 4x4 block

    default:
      throw std::runtime_error("Unsupported DXGI_FOR,MAT");
    }
  }
};

class Material
{
private:
  std::vector<Texture*> _textures;

public:
  Material() {}
  ~Material()
  {
    if (!_textures.empty())
    {
      std::ranges::for_each(_textures, [](Texture* tex) { SAFE_RELEASE(tex); });
    }
  }

public:
  void CreateMaterial(Device* device, MaterialData data)
  {
    if (data.albedoTexture != Handle::kInvalidHandle)
    {
      Texture* albedo =
          Texture::CreateSRV(device, AccessTextureData(data.albedoTexture));
      _textures.push_back(albedo);
    }
    if (data.normalTexture != Handle::kInvalidHandle)
    {
      Texture* normal =
          Texture::CreateSRV(device, AccessTextureData(data.normalTexture));
      _textures.push_back(normal);
    }
    if (data.metallicRoughnessTexture != Handle::kInvalidHandle)
    {
      Texture* metallicRoughness = Texture::CreateSRV(
          device, AccessTextureData(data.metallicRoughnessTexture));
      _textures.push_back(metallicRoughness);
    }
    if (data.emissiveTexture != Handle::kInvalidHandle)
    {
      Texture* emissive =
          Texture::CreateSRV(device, AccessTextureData(data.emissiveTexture));
      _textures.push_back(emissive);
    }
  }
  int Size() { return _textures.size(); }
  void PSSetResourceViews(Device* device)
  {
    std::vector<ID3D11ShaderResourceView*> srvs;
    std::ranges::for_each(_textures, [&srvs](Texture* tex) {
      srvs.push_back(tex->GetResource().Get());
    });
    device->GetImmContext()->PSSetShaderResources(0, srvs.size(), srvs.data());
  }
  void PSSetResourceView(Device* device, Renderer::TextureType type)
  {
    device->GetImmContext()->PSSetShaderResources(
        static_cast<UINT>(type), 1,
        _textures[static_cast<int>(type)]->GetResource().GetAddressOf());
  }
};