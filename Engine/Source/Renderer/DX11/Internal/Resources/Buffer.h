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
  float ambientIntencity;
  float emissiveIntencity;
  float screenWidth;
  float screenHeight;
};

struct World
{
  Matrix world;
  //Color color;
};

struct PixelData
{
  float alphaCutoff=0.f;
  float metalicFactor = 0.f;
  float roughnessFactor = 0.f;
  float padding1;
  Color albedoFactor;
};
struct BoneMatrix
{
  XMMATRIX matrix[512] = {};
};

struct SSAOParames
{
  Vector2 noiseScale;
  float radius;        // »ùÇÃ¸µ ¹Ý°æ
  float nearplane;
  float farplane;
  Vector3 padding3;
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
  ComPtr<ID3D11Buffer> boneIDBuffer;
  ComPtr<ID3D11Buffer> boneWeightsBuffer;
  ComPtr<ID3D11ShaderResourceView> boneIDSrv;
  ComPtr<ID3D11ShaderResourceView> boneWeightsSrv;
  Matrix world=Matrix::Identity;
  std::vector<XMMATRIX> boneMatirx = std::vector<XMMATRIX>();
  Material* material=nullptr;
  RenderPassFlags flags = 0;
};

namespace RenderMesh
{
struct StaticMesh
{
  MeshBuffer* buffer = nullptr;
  Matrix world;
  Color outlineColor;
};
struct SkelMesh
{
  MeshBuffer* buffer = nullptr;
  Matrix world;
  std::vector<XMMATRIX> boneMatrix;
  Color outlineColor;
};
}