#pragma once
#include "../Common.h"
#include "../Device.h"
#include "../Resources/Material.h"
#include "../Types.h"
#include "directxtk/SimpleMath.h"

using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

namespace Constant
{
struct Frame
{
  Light::DirectionalLight mainDirectionalLight;
  Vector4 cameraPosition;
  Matrix view;
  Matrix projection;
  Matrix inverseView;
  Matrix inverseProjection;
};

struct World
{
  Matrix world;
};

} // namespace Constant

struct MeshBuffer
{
  ~MeshBuffer() { SAFE_RELEASE(material); }

  ComPtr<ID3D11Buffer> vertexBuffer;
  ComPtr<ID3D11Buffer> indexBuffer;
  UINT stride;
  UINT offset;
  UINT nIndices;
  Matrix world;
  Material* material;
  RenderPassFlags flags = 0;
};