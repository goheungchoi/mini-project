#pragma once
#include "../Common.h"
#include "../Device.h"
#include "Core/Common.h"
namespace Quad
{
struct Vertex
{
  Vector4 position;
  Vector2 uv;
};
class QuadFrame
{
public:
  ComPtr<ID3D11Buffer> _vertexBuffer;
  ComPtr<ID3D11Buffer> _indexBuffer;
  ComPtr<ID3D11Buffer> _constant;
  int _vertexCount = 0;
  int _indexCount = 0;
  Device* _device = nullptr;

public:
  // SWTODO : √ ±‚»≠
  QuadFrame(Device* device) : _device{device} {}
  ~QuadFrame() {}

public:
  void InitializeBuffers()
  {
    Quad::Vertex* vertices;
    std::vector<UINT> indices;
    _vertexCount = 4;
    _indexCount = 6;
    vertices = new Quad::Vertex[_vertexCount];
    vertices[0].position =
        DirectX::SimpleMath::Vector4(-1.0f, 1.0f, 0.0f, 1.0f); // Top left
    vertices[1].position =
        DirectX::SimpleMath::Vector4(1.0f, 1.0f, 0.0f, 1.0f); // Top right
    vertices[2].position =
        DirectX::SimpleMath::Vector4(-1.0f, -1.0f, 0.0f, 1.0f); // Bottom left
    vertices[3].position =
        DirectX::SimpleMath::Vector4(1.0f, -1.0f, 0.0f, 1.0f); // Bottom right
    
    vertices[0].uv = DirectX::SimpleMath::Vector2(0.0f, 0.0f); // Top left
    vertices[1].uv = DirectX::SimpleMath::Vector2(1.0f, 0.0f); // Top right
    vertices[2].uv = DirectX::SimpleMath::Vector2(0.0f, 1.0f); // Bottom left
    vertices[3].uv = DirectX::SimpleMath::Vector2(1.0f, 1.0f); // Bottom right
    indices = {0, 1, 2, 2, 1, 3};
    _vertexBuffer =
        _device->CreateDataBuffer(vertices, sizeof(Quad::Vertex) * _vertexCount,
                                  D3D11_BIND_VERTEX_BUFFER);
    _indexBuffer = _device->CreateDataBuffer(
        indices.data(), sizeof(UINT) * _indexCount, D3D11_BIND_INDEX_BUFFER);
    delete[] vertices;
    vertices = 0;
  }
};
} // namespace Quad
