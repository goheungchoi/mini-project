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
    D3D11_TEXTURE2D_DESC desc = CreateTexture2DDesc(
        data.width, data.height, data.format, data.mipLevels,
        D3D11_BIND_SHADER_RESOURCE, data.arrayLayers);
    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = data.ddsData.data();
    initData.SysMemPitch = data.width * 4;
    ComPtr<ID3D11Texture2D> texture;
    HR_T(device->GetDevice()->CreateTexture2D(&desc, &initData,
                                              texture.GetAddressOf()));
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = data.format;
    srvDesc.ViewDimension = data.isCubeMap ? D3D11_SRV_DIMENSION_TEXTURECUBE
                                           : D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = data.mipLevels;
    HR_T(device->GetDevice()->CreateShaderResourceView(
        texture.Get(), &srvDesc, pInstance->_resource.GetAddressOf()));
    return pInstance;
  }
  ComPtr<ID3D11ShaderResourceView> GetResource() { return _resource; }
};

class Material
{
private:
  std::vector<Texture*> _textures;

public:
  Material() {}
  ~Material()
  {
    std::ranges::for_each(_textures, [](Texture* tex) { SAFE_RELEASE(tex); });
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