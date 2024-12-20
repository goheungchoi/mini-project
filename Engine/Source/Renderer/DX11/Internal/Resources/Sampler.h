#pragma once;
#include "../Common.h"
using namespace Microsoft::WRL;

enum SamplerType
{
  LINEAR_WRAP,
  LINEAR_CLAMP,
  ANIMSOROPIC,
  COMPARISON
};

struct Sampler
{
  SamplerType type;
  ComPtr<ID3D11SamplerState> state;
};