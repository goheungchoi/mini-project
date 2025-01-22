#include "../Common.h"
#include "../Device.h"
#include "../Types.h"
#include "Buffer.h"
#include "Core/Common.h"

namespace Geometry
{
struct Sphere
{
  MeshBuffer* buffer;
};
struct Box
{
  MeshBuffer* buffer;
};
struct Cylinder
{
  MeshBuffer* buffer;
};
} // namespace Geometry

class GeometryPrimitive
{
private:
  Device* _device;

public:
  Geometry::Sphere sphere;
  Geometry::Box box;
  Geometry::Cylinder cylinder;

public:
  GeometryPrimitive(Device* device) : _device{device}
  {
    sphere.buffer = new MeshBuffer;
    box.buffer = new MeshBuffer;
    cylinder.buffer = new MeshBuffer;
    ModelHandle modelHandle = LoadModel("Models\\Sphere\\Sphere.obj");
    ModelData modelData = AccessModelData(modelHandle);
    MeshData meshData;
    std::ranges::for_each(modelData.meshes, [&](MeshHandle handle) {
      meshData = AccessMeshData(handle);
    });
    uint32_t size = sizeof(Vertex) * meshData.vertices.size();
    sphere.buffer->vertexBuffer = _device->CreateDataBuffer(
        meshData.vertices.data(), size, D3D11_BIND_VERTEX_BUFFER);
    size = sizeof(UINT) * meshData.indices.size();
    sphere.buffer->indexBuffer = _device->CreateDataBuffer(
        meshData.indices.data(), size, D3D11_BIND_INDEX_BUFFER);
    sphere.buffer->nIndices = meshData.indices.size();
    sphere.buffer->stride = sizeof(Vertex);
    sphere.buffer->offset = 0;
    sphere.buffer->flags |= RenderPassType::kWireFrame;

    modelHandle = LoadModel("Models\\Box\\Box.fbx");
    modelData = AccessModelData(modelHandle);
    std::ranges::for_each(modelData.meshes, [&](MeshHandle handle) {
      meshData = AccessMeshData(handle);
    });
     size = sizeof(Vertex) * meshData.vertices.size();
    box.buffer->vertexBuffer = _device->CreateDataBuffer(
        meshData.vertices.data(), size, D3D11_BIND_VERTEX_BUFFER);
    size = sizeof(UINT) * meshData.indices.size();
    box.buffer->indexBuffer = _device->CreateDataBuffer(
        meshData.indices.data(), size, D3D11_BIND_INDEX_BUFFER);
    box.buffer->nIndices = meshData.indices.size();
    box.buffer->stride = sizeof(Vertex);
    box.buffer->offset = 0;
    box.buffer->flags |= RenderPassType::kWireFrame;
    
    modelHandle = LoadModel("Models\\Cylinder\\Cylinder.fbx");
    modelData = AccessModelData(modelHandle);
    std::ranges::for_each(modelData.meshes, [&](MeshHandle handle) {
      meshData = AccessMeshData(handle);
    });
     size = sizeof(Vertex) * meshData.vertices.size();
    cylinder.buffer->vertexBuffer = _device->CreateDataBuffer(
        meshData.vertices.data(), size, D3D11_BIND_VERTEX_BUFFER);
    size = sizeof(UINT) * meshData.indices.size();
    cylinder.buffer->indexBuffer = _device->CreateDataBuffer(
        meshData.indices.data(), size, D3D11_BIND_INDEX_BUFFER);
    cylinder.buffer->nIndices = meshData.indices.size();
    cylinder.buffer->stride = sizeof(Vertex);
    cylinder.buffer->offset = 0;
    cylinder.buffer->flags |= RenderPassType::kWireFrame;
  }
  ~GeometryPrimitive()
  {
    SAFE_RELEASE(sphere.buffer);
    SAFE_RELEASE(box.buffer);
    SAFE_RELEASE(cylinder.buffer);
  }
};