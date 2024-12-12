#pragma once
#include "../Common.h"
using namespace Microsoft::WRL;
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
  void SetInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& desc)
  {
    
  }

public:
  ComPtr<ID3D11RasterizerState> _rasterizeState = nullptr;
  ComPtr<ID3D11DepthStencilState> _depthStencilState = nullptr;
  ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
  ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
  ComPtr<ID3D11InputLayout> _pinputLayout = nullptr;
  D3D_PRIMITIVE_TOPOLOGY _topology;
  
};