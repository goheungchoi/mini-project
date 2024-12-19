#pragma once
#include "../Common.h"
#include "../Descriptions.h"
using namespace Microsoft::WRL;

//�켱�� �� pipe line�� �ϳ��� ���̴��� ����غ���...
class PipelineState
{
public:
  PipelineState() = default;
  ~PipelineState() = default;

public:
  void SetInputTopology(D3D_PRIMITIVE_TOPOLOGY topology)
  {
    _topology = topology;
  }

public:
  ComPtr<ID3D11RasterizerState> _rasterizeState = nullptr;
  ComPtr<ID3D11DepthStencilState> _depthStencilState = nullptr;
  ComPtr<ID3D11BlendState> _blendState = nullptr;
  D3D_PRIMITIVE_TOPOLOGY _topology;
  
};