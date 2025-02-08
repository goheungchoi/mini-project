#pragma once
#include "Core/Common.h"
#include "Core/Types/Trail.h"
#include "Renderer/DX11/Internal/Common.h"
#include "Renderer/DX11/Internal/Device.h"

struct TrailVertex
{
  Vector4 position;
  // Vector2 uv;
  float alpha;
};

class TrailResource
{
private:
  Device* _device{nullptr};

public:
  ComPtr<ID3D11Buffer> _vertexBuffer;
  UINT stride{0};
  UINT offset{0};
  UINT vertexCount{0};

public:
  TrailResource(Device* device) : _device{device}
  {
    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = sizeof(TrailVertex) * 1000;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    HR_T(_device->GetDevice()->CreateBuffer(&desc, nullptr,
                                            _vertexBuffer.GetAddressOf()));
    stride = sizeof(TrailVertex);
    offset = 0;
  }
  ~TrailResource() {};

public:
  void UpdateTrailResource(const std::deque<TrailPoint>& trailPoint,
                           float maxDuration, float width, Vector4 direction,
                           Vector4 eye)
  {
    std::vector<TrailVertex> vertices;
    for (size_t i = 0; i < trailPoint.size(); i++)
    {
      float alpha = 1.f - (trailPoint[i].age / maxDuration);
      alpha = std::clamp(alpha, 0.f, 1.f);
      float scaleWidth = width * alpha;
      Vector3 dir = (Vector3)direction;
      dir.Normalize();

      Vector3 toCamera = eye - trailPoint[i].position;
      toCamera.Normalize();

      Vector3 rightVec = dir.Cross(toCamera);
      rightVec.Normalize();

      Vector3 offset = rightVec * scaleWidth * width;

      TrailVertex left, right;
      left.position = Vector4(trailPoint[i].position.x - offset.x,
                              trailPoint[i].position.y - offset.y,
                              trailPoint[i].position.z - offset.z, 1.f);
      right.position = Vector4(trailPoint[i].position.x + offset.x,
                               trailPoint[i].position.y + offset.y,
                               trailPoint[i].position.z + offset.z, 1.f);
      left.alpha = alpha;
      right.alpha = alpha;

      vertices.push_back(left);
      vertices.push_back(right);
    }
    vertexCount = vertices.size();
    ID3D11DeviceContext* dc = _device->GetImmContext();
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HR_T(dc->Map(_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0,
                 &mappedResource));
    memcpy(mappedResource.pData, vertices.data(),
           sizeof(TrailVertex) * vertices.size());
    dc->Unmap(_vertexBuffer.Get(), 0);
  }
};