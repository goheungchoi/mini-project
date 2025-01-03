#pragma
#include "../Common.h"
#include "../Device.h"
#include "../Types.h"
#include "directxtk/SimpleMath.h"

using namespace Microsoft::WRL;

struct MeshBuffer
{
  ComPtr<ID3D11Buffer> vertexBuffer;
  ComPtr<ID3D11Buffer> indexBuffer;
  UINT stride;
  UINT offset;
  UINT nIndices;
  RenderPassFlags flags;
};

namespace Constant
{
struct WVP
{
  Matrix world;
  Matrix view;
  Matrix projection;
};
} // namespace Constant

/*
Current Frmae Buffer
:stores information about the current frame's constant buffers

*/
class FrameBuffer
{
private:
  ComPtr<ID3D11Buffer> _currWVP;

public:
  FrameBuffer(Device* device)
  {
    _currWVP = device->CreateConstantBuffer<Constant::WVP>();
  }
  ~FrameBuffer() = default;

public:
  
};