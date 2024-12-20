#pragma once
#include "../Common.h"

//우선은 각 pipe line에 하나의 쉐이더만 사용해보자...
//class PipelineState
//{
//public:
//  PipelineState() = default;
//  ~PipelineState() = default;
//
//public:
//  void CreateViewPort(FLOAT TopLeftX, FLOAT TopLeftY,FLOAT Width, FLOAT Height,
//                      FLOAT MinDepth, FLOAT MaxDepth) 
//  {
//    _viewPort = {
//        TopLeftX, TopLeftY, Width, Height, MinDepth, MaxDepth
//    };
//  }
//
//  
//public:
//  ComPtr<ID3D11RasterizerState> _rasterizeState = nullptr;
//  ComPtr<ID3D11DepthStencilState> _depthStencilState = nullptr;
//  ComPtr<ID3D11BlendState> _blendState = nullptr;
//  D3D11_VIEWPORT _viewPort;
//};