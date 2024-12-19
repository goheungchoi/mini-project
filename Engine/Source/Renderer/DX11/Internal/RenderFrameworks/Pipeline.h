#pragma once
#include "../Common.h"
#include "../Descriptions.h"
using namespace Microsoft::WRL;

//우선은 각 pipe line에 하나의 쉐이더만 사용해보자...
class PipelineState
{
public:
  PipelineState() = default;
  ~PipelineState() = default;

public:
  
public:
  ComPtr<ID3D11RasterizerState> _rasterizeState = nullptr;
  ComPtr<ID3D11DepthStencilState> _depthStencilState = nullptr;
  ComPtr<ID3D11BlendState> _blendState = nullptr;
};