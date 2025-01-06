#pragma
#include "../Common.h"
#include "../Device.h"
#include "../Types.h"
#include "directxtk/SimpleMath.h"

using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

namespace Constant
{
struct WVP
{
  Matrix world;
  Matrix view;
  Matrix projection;
};
} // namespace Constant


struct MeshBuffer
{
  ComPtr<ID3D11Buffer> vertexBuffer;
  ComPtr<ID3D11Buffer> indexBuffer;
  UINT stride;
  UINT offset;
  UINT nIndices;
  Matrix world;
  RenderPassFlags flags;
};