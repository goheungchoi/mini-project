#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../Resources/Material.h"
#include "../Types.h"

using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

namespace Constant
{
struct Frame
{
  DirectionalLight mainDirectionalLight;
  Vector4 cameraPosition;
  Matrix view;
  Matrix projection;
  Matrix inverseView;
  Matrix inverseProjection;
  Matrix shadwoView;
  Matrix shadowProjection;
};

struct World
{
  Matrix world;
  Color color;
};

struct PixelData
{
  float alphaCutoff=0.f;
  Vector3 padding1{};
  Color color;
};
} // namespace Constant

struct MeshBuffer
{
  ~MeshBuffer()
  { 
    SAFE_RELEASE(material); 
  }

  ComPtr<ID3D11Buffer> vertexBuffer;
  ComPtr<ID3D11Buffer> indexBuffer;
  UINT stride;
  UINT offset;
  UINT nIndices;
  Matrix world=Matrix::Identity;
  Material* material=nullptr;
  RenderPassFlags flags = 0;
};