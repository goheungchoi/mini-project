#pragma
#include "../Common.h"
using namespace Microsoft::WRL;

struct MeshBuffer
{
  ComPtr<ID3D11Buffer> vertexBuffer;
  ComPtr<ID3D11Buffer> indexBuffer;
  UINT stride;
  UINT offset;
  UINT nIndices;
};