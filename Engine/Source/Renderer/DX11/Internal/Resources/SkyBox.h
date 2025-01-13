#include "../Common.h"
#include "../Device.h"
#include "Buffer.h"
#include "Core/Common.h"
#include "Material.h"

class SkyBox
{
private:
  Device* _device;
  MeshBuffer* _mesh;
  std::vector<Texture*> _textures;

public:
  SkyBox(Device* device) : _device{device} {}
  ~SkyBox() { SAFE_RELEASE(_mesh); }

public:
  void Init()
  {
    _mesh = new MeshBuffer;
    ModelHandle modelHandle = LoadModel("Models\\Skybox\\Skybox.obj");
    ModelData modelData = AccessModelData(modelHandle);
    MeshData meshData;
    std::ranges::for_each(modelData.meshes, [&](MeshHandle meshHandle) {
      meshData = AccessMeshData(meshHandle);
    });
    uint32_t size = sizeof(Vertex) * meshData.vertices.size();
    _mesh->vertexBuffer = _device->CreateDataBuffer(
        meshData.vertices.data(), size, D3D11_BIND_VERTEX_BUFFER);
    size = sizeof(UINT) * meshData.indices.size();
    _mesh->indexBuffer = _device->CreateDataBuffer(
        meshData.indices.data(), size, D3D11_BIND_INDEX_BUFFER);
    _mesh->nIndices = meshData.indices.size();
    _mesh->stride = sizeof(Vertex);
    _mesh->offset = 0;
    Matrix world = Matrix::Identity;
    Matrix scale = Matrix::CreateScale(100.f);
    _mesh->world = world * scale;
    _mesh->flags |= RenderPassType::OpaquePass;
  }
  void SetTexture(LPCSTR envPath, LPCSTR specularBRDFPath,
                  LPCSTR diffuseIrrPath, LPCSTR specularIBLPath)
  {
    _textures.clear();
    _textures.reserve(4);
    TextureHandle handle = LoadTexture(envPath, TextureType::kUnknown);
    TextureData data;
    Texture* texture;
    if (handle != Handle::kInvalidHandle)
    {
      data = AccessTextureData(handle);
      texture = Texture::CreateSRV(_device, data);
      _textures.push_back(texture);
    }
    handle = LoadTexture(specularBRDFPath, TextureType::kUnknown);
    if (handle != Handle::kInvalidHandle)
    {
      data = AccessTextureData(handle);
      texture = Texture::CreateSRV(_device, data);
      _textures.push_back(texture);
    }
    handle = LoadTexture(diffuseIrrPath, TextureType::kUnknown);
    if (handle != Handle::kInvalidHandle)
    {
      data = AccessTextureData(handle);
      texture = Texture::CreateSRV(_device, data);
      _textures.push_back(texture);
    }
    handle = LoadTexture(specularIBLPath, TextureType::kUnknown);
    if (handle != Handle::kInvalidHandle)
    {
      data = AccessTextureData(handle);
      texture = Texture::CreateSRV(_device, data);
      _textures.push_back(texture);
    }
    std::vector<ID3D11ShaderResourceView*> srvs;
    std::ranges::for_each(_textures, [&srvs](Texture* tex) {
      srvs.push_back(tex->GetResource().Get());
    });
    _device->GetImmContext()->PSSetShaderResources(5, srvs.size(), srvs.data());
  }
  void Render()
  {
    _device->GetImmContext()->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _device->GetImmContext()->IASetVertexBuffers(
        0, 1, _mesh->vertexBuffer.GetAddressOf(), &_mesh->stride,
        &_mesh->offset);
    _device->GetImmContext()->IASetIndexBuffer(_mesh->indexBuffer.Get(),
                                               DXGI_FORMAT_R32_UINT, 0);
    _device->GetImmContext()->DrawIndexed(_mesh->nIndices, 0, 0);
  }
  MeshBuffer* GetMesh() { return _mesh; }
};