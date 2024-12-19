#pragma
#include "../Common.h"
using namespace Microsoft::WRL;

struct Texture
{
  ComPtr<ID3D11Texture2D> texture;
  ComPtr<ID3D11ShaderResourceView> srv;
  ComPtr<ID3D11UnorderedAccessView> uav;
  UINT width, height;
  UINT levels;
};

struct Material
{
  Texture albedoTexture;
  Texture normalTexture;
  Texture emissiveTexture;
  //Texture 
};